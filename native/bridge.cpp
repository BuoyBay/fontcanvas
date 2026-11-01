#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <stdint.h>
#include <cmath>
#include <cstring>
#include <string>
#include <vector>
#include <new>

using namespace Gdiplus;
#define API extern "C" __declspec(dllexport)
struct Metrics { double x,y,width,height,ascent,descent,spacing; };
struct Canvas { ULONG_PTR token=0; PrivateFontCollection *collection=nullptr; std::vector<std::wstring> names; std::string error; };

static bool wide(const char *s,std::wstring &out) {
    if(!s) return false;
    int n=MultiByteToWideChar(CP_UTF8,MB_ERR_INVALID_CHARS,s,-1,nullptr,0);
    if(!n) return false;
    std::vector<wchar_t> b((size_t)n);
    if(!MultiByteToWideChar(CP_UTF8,MB_ERR_INVALID_CHARS,s,-1,b.data(),n)) return false;
    out.assign(b.data()); return true;
}
static std::string utf8(const wchar_t *s) {
    int n=WideCharToMultiByte(CP_UTF8,0,s,-1,nullptr,0,nullptr,nullptr);
    if(n<=0) return {};
    std::vector<char> b((size_t)n);
    if(!WideCharToMultiByte(CP_UTF8,0,s,-1,b.data(),n,nullptr,nullptr)) return {};
    return std::string(b.data());
}
API void *bfc_open(const char *path) noexcept {
    Canvas *c=new(std::nothrow) Canvas;
    if(!c) return nullptr;
    try {
        std::wstring p;
        if(!wide(path,p)||p.empty()) { c->error="Invalid UTF-8 font path"; return c; }
        GdiplusStartupInput input;
        if(GdiplusStartup(&c->token,&input,nullptr)!=Ok) { c->error="GDI+ startup failed"; return c; }
        c->collection=new PrivateFontCollection;
        if(c->collection->AddFontFile(p.c_str())!=Ok) { c->error="GDI+ rejected private font file"; return c; }
        int count=c->collection->GetFamilyCount();
        if(count<=0||count>4096) { c->error="Private font file has no bounded family list"; return c; }
        std::vector<FontFamily> families((size_t)count);
        int found=0;
        if(c->collection->GetFamilies(count,families.data(),&found)!=Ok) { c->error="Private family enumeration failed"; return c; }
        for(int i=0;i<found;i++) { WCHAR name[LF_FACESIZE]={0}; if(families[i].GetFamilyName(name)!=Ok) { c->error="Private family name failed"; return c; } c->names.emplace_back(name); }
    } catch(...) { c->error="Private font allocation failed"; }
    return c;
}
API const char *bfc_error(Canvas *c) noexcept { return c?c->error.c_str():"Private font allocation failed"; }
API void bfc_close(Canvas *c) noexcept { if(!c)return; delete c->collection; if(c->token)GdiplusShutdown(c->token); delete c; }
API int64_t bfc_count(Canvas *c) noexcept { return c&&!c->error.size()?(int64_t)c->names.size():0; }
API int64_t bfc_name(Canvas *c,int64_t index,char *buffer,int64_t capacity) noexcept {
    if(!c||index<0||(uint64_t)index>=c->names.size()||!buffer||capacity<1)return 0;
    try { std::string name=utf8(c->names[(size_t)index].c_str()); if(name.size()+1>(uint64_t)capacity)return 0; memcpy(buffer,name.c_str(),name.size()+1);return (int64_t)name.size(); }catch(...){return 0;}
}
API int64_t bfc_style(Canvas *c,int64_t index,int64_t flags) noexcept {
    if(!c||!c->error.empty()||index<0||(uint64_t)index>=c->names.size()||flags<0||flags>3)return 0;
    FontFamily family(c->names[(size_t)index].c_str(),c->collection);
    return family.GetLastStatus()==Ok&&family.IsStyleAvailable((INT)flags)?1:0;
}
/* All dimensions/metrics are raster pixels. The caller owns its RGBA buffer.
 * No private fonts are registered in GDI or the system font database. */
API int64_t bfc_layout(Canvas *c,const char *text,int64_t index,double size,int64_t flags,
                       int64_t width,int64_t height,double x,double baseline,
                       double r,double g,double b,double a,unsigned char *rgba,
                       int64_t capacity,Metrics *out) noexcept {
    if(out)memset(out,0,sizeof(*out));
    if(!c||!c->error.empty()||!out||!text||index<0||(uint64_t)index>=c->names.size()||
       !std::isfinite(size)||size<=0||size>16384||flags<0||flags>7||
       !std::isfinite(x)||!std::isfinite(baseline)||
       !std::isfinite(r)||!std::isfinite(g)||!std::isfinite(b)||!std::isfinite(a)||
       r<0||r>1||g<0||g>1||b<0||b>1||a<0||a>1||strlen(text)>1048576)return 1;
    bool draw=rgba!=nullptr;
    if(draw&&(width<1||height<1||width>16384||height>16384||width*height>16777216||capacity<width*height*4))return 1;
    try {
        std::wstring value;
        if(!wide(text,value)||value.size()*size>262144.0)return 1;
        // This API renders one explicitly positioned line; it does not wrap.
        for(wchar_t ch:value)if(ch=='\n'||ch=='\r'||ch=='\t'||ch==0x2028||ch==0x2029)return 1;
        FontFamily family(c->names[(size_t)index].c_str(),c->collection);
        if(family.GetLastStatus()!=Ok)return 3;
        if(!family.IsStyleAvailable((INT)(flags&3)))return 2;
        Font font(&family,(REAL)size,(INT)flags,UnitPixel);
        if(font.GetLastStatus()!=Ok)return 3;
        Bitmap bitmap(draw?(INT)width:1,draw?(INT)height:1,PixelFormat32bppARGB);
        if(bitmap.GetLastStatus()!=Ok)return 3;
        Graphics graphics(&bitmap);
        if(graphics.GetLastStatus()!=Ok||graphics.Clear(Color(0,0,0,0))!=Ok||
           graphics.SetTextRenderingHint(TextRenderingHintAntiAliasGridFit)!=Ok)return 3;
        StringFormat format(StringFormat::GenericTypographic());
        if(format.GetLastStatus()!=Ok||format.SetFormatFlags(format.GetFormatFlags()|StringFormatFlagsMeasureTrailingSpaces|StringFormatFlagsNoWrap|StringFormatFlagsNoClip)!=Ok)return 3;
        UINT16 em=family.GetEmHeight((INT)(flags&3));
        if(!em)return 3;
        double ascent=family.GetCellAscent((INT)(flags&3))*size/em;
        double descent=family.GetCellDescent((INT)(flags&3))*size/em;
        double spacing=family.GetLineSpacing((INT)(flags&3))*size/em;
        RectF bounds;
        if(!value.empty()&&graphics.MeasureString(value.c_str(),(INT)value.size(),&font,PointF(0,0),&format,&bounds)!=Ok)return 3;
        *out={bounds.X,bounds.Y-ascent,bounds.Width,bounds.Height,ascent,descent,spacing};
        if(draw) {
            Color color((BYTE)std::lround(a*255),(BYTE)std::lround(r*255),(BYTE)std::lround(g*255),(BYTE)std::lround(b*255));
            SolidBrush brush(color);
            if(!value.empty()&&graphics.DrawString(value.c_str(),(INT)value.size(),&font,PointF((REAL)x,(REAL)(baseline-ascent)),&format,&brush)!=Ok)return 3;
            BitmapData bits={}; Rect rect(0,0,(INT)width,(INT)height);
            if(bitmap.LockBits(&rect,ImageLockModeRead,PixelFormat32bppARGB,&bits)!=Ok)return 3;
            // ARGB LockBits supplies straight-alpha BGRA bytes, including negative stride.
            for(int64_t y=0;y<height;y++) {
                const unsigned char *row=(const unsigned char *)bits.Scan0+y*bits.Stride;
                for(int64_t px=0;px<width;px++) {
                    unsigned char *target=rgba+(y*width+px)*4;
                    target[0]=row[px*4+2];target[1]=row[px*4+1];target[2]=row[px*4];target[3]=row[px*4+3];
                    if(!target[3])target[0]=target[1]=target[2]=0;
                }
            }
            if(bitmap.UnlockBits(&bits)!=Ok)return 3;
        }
        return 0;
    } catch(...) {return 3;}
}
