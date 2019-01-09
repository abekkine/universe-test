#ifndef VIEWPORT_HPP_
#define VIEWPORT_HPP_

#include <GL/glut.h>

class Viewport {
public:
    Viewport() {
        left_ = -1.0;
        right_ = 1.0;
        bottom_ = -1.0;
        top_ = 1.0;
        near_ = -1.0;
        far_ = 1.0;
        center_x_ = 0.0;
        center_y_ = 0.0;
        pixel_size_ = 0.0015;
    }
    ~Viewport() {}
    void SetWindowSize(const double & w, const double & h) {
        window_width_ = w;
        window_height_ = h;
        zoom_max_distance_ = FastSqrt(w * w + h * h) >> 1;
        Update();
    }
    void Ortho() {
        glOrtho(left_, right_, bottom_, top_, near_, far_);
    }
    void Update() {
        left_ = center_x_ - 0.5 * pixel_size_ * window_width_;
        right_ = center_x_ + 0.5 * pixel_size_ * window_width_;
        bottom_ = center_y_ - 0.5 * pixel_size_ * window_height_;
        top_ = center_y_ + 0.5 * pixel_size_ * window_height_;
    }
    void PanStart(const int x, const int y) {
        pan_mode_ = true;
        pan_start_x_ = x;
        pan_start_y_ = y;
        pan_start_center_x_ = center_x_;
        pan_start_center_y_ = center_y_;
    }
    void PanStop() {
        pan_mode_ = false;
    }
    void Pan(const int x, const int y) {
        if (pan_mode_ == false) {
            return;
        }

        int dx = x - pan_start_x_;
        int dy = y - pan_start_y_;

        center_x_ = pan_start_center_x_ - dx * pixel_size_;
        center_y_ = pan_start_center_y_ + dy * pixel_size_;

        Update();
    }
    void ZoomStart(const int x, const int y) {
        zoom_mode_ = true;
        zoom_start_distance_ = GetZoomDistance(x, y);
        zoom_start_pixel_size_ = pixel_size_;
    }
    void ZoomStop() {
        zoom_mode_ = false;
    }
    void Zoom(const int x, const int y) {
        if (zoom_mode_ == false) {
            return;
        }

        zoom_distance_ = GetZoomDistance(x, y);

        double zoom_ratio = (float)(zoom_distance_ - zoom_start_distance_)/(float)zoom_max_distance_;
        pixel_size_ = zoom_start_pixel_size_ * (1.0 - zoom_ratio);

        Update();
    }
private:
    int GetZoomDistance(const int x, const int y) {
        int dx = x - 0.5 * window_width_;
        int dy = y - 0.5 * window_height_;
        return FastSqrt(dx * dx + dy * dy);
    }
    uint32_t FastSqrt(uint32_t value) {
        register uint32_t root, remainder, place;
        root = 0;
        remainder = value;
        place = 0x4000000;

        while (place > remainder) {
            place = place >> 2;
        }

        while (place) {
            if (remainder >= root + place) {
                remainder -= root + place;
                root += place << 1;
            }
            root = root >> 1;
            place = place >> 2;
        }

        return root;
    }

private:
    // World center
    double center_x_;
    double center_y_;
    // Pixel size
    double pixel_size_;
    // Viewport limits
    double left_;
    double right_;
    double bottom_;
    double top_;
    double near_;
    double far_;
    // Window size for calculations
    double window_width_;
    double window_height_;
    // Pan Mode
    bool pan_mode_;
    int pan_start_x_;
    int pan_start_y_;
    double pan_start_center_x_;
    double pan_start_center_y_;
    // Zoom Mode
    bool zoom_mode_;
    int zoom_start_distance_;
    double zoom_start_pixel_size_;
    int zoom_distance_;
    int zoom_max_distance_;
};

#endif // VIEWPORT_HPP_
