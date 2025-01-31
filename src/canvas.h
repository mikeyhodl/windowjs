#ifndef WINDOWJS_CANVAS_H
#define WINDOWJS_CANVAS_H

#include <skia/include/core/SkCanvas.h>
#include <skia/include/core/SkImage.h>
#include <skia/include/core/SkSurface.h>
#include <skia/include/gpu/GrDirectContext.h>
#include <skia/include/gpu/gl/GrGLInterface.h>

class Window;

class CanvasSharedContext final {
 public:
  explicit CanvasSharedContext(Window* window);
  ~CanvasSharedContext();

  void Flush();

  Window* owner() const { return owner_; }
  GrDirectContext* skia_context() const { return gr_context_.get(); }

 private:
  Window* owner_;

  sk_sp<const GrGLInterface> gr_interface_;
  sk_sp<GrDirectContext> gr_context_;
};

class Canvas final {
 public:
  enum Target {
    FRAMEBUFFER_0,
    TEXTURE,
  };

  Canvas(CanvasSharedContext* shared_context, int width, int height,
         Target target);

  ~Canvas();

  int width() const { return width_; }
  int height() const { return height_; }
  SkSurface* surface() const { return surface_.get(); }
  SkCanvas* canvas() const { return surface_->getCanvas(); }
  CanvasSharedContext* shared_context() const { return shared_context_; }

  void Reset();
  void Resize(int width, int height);

  sk_sp<SkImage> MakeImageSnapshot();

  void ReadPixels(int x, int y, int width, int height, void* destination);
  void WritePixels(const void* pixels, int x, int y, int width, int height,
                   int row_stride);

 private:
  CanvasSharedContext* shared_context_;
  sk_sp<SkSurface> surface_;
  GrBackendTexture texture_;
  int width_;
  int height_;
  Target target_;
};

#endif  // WINDOWJS_CANVAS_H
