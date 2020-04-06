#include <image.h>
#include <io_tools.h>

#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>

namespace igg {

// Create empty image
Image::Image() = default;

// Create image with rowsxcoluns pixels
Image::Image(const int &rows, const int &cols) : rows_(rows), cols_(cols) {
  data_.resize(rows_ * cols_);
}
const int &Image::rows() const { return rows_; }
const int &Image::cols() const { return cols_; }

// Setter and Getter
// int val = image.at(row, col);
// image.at(row, col) = 255;
int &Image::at(int row, int col) { return data_.at(col + row * cols_); }

bool Image::FillFromPgm(const std::string &file_name) {
  io_tools::ImageData reading_image = io_tools::ReadFromPgm(file_name);
  if (reading_image.rows != 0)  // if the image is empty (error on reading)
  {
    cols_ = std::move(reading_image.rows);
    rows_ = std::move(reading_image.cols);
    max_val_ = std::move(reading_image.max_val);
    data_ = std::move(reading_image.data);
    return true;
  }
  return false;
}
void Image::WriteToPgm(const std::string &file_name) const {
  // std::cout << "WRITING A " << rows_ << " X " << cols_ << "Image\n";
  io_tools::WriteToPgm({cols_, rows_, max_val_, data_}, file_name);
}

std::vector<float> Image::ComputeHistogram(const int bins) const {
  // Initialize histogram with zeros
  std::vector<float> histogram(bins, 0.);

  // find threshold for bins
  float div = (float)max_val_ / (float)bins;

  // loop over pixels and allocate them into the respective bin
  for (auto pixel : data_) {
    int location;
    if (pixel >= max_val_)
      location = bins - 1;
    else
      location = (int)(floor((float)pixel / div));  //  find the correct bin
    histogram.at(location) += 1.;
  }

  const int &size = data_.size();
  std::for_each(histogram.begin(), histogram.end(),
                [size](float &n) { n /= ((float)size); });

  return histogram;
}

void Image::DownScale(int scale) {
  int tmp_cols = cols_;
  // new image sizes
  rows_ /= scale;
  cols_ /= scale;
  int new_size = rows_ * cols_;

  // temp image data
  std::vector<int> temp_image(new_size, 0);

  for (int r = 0; r < rows_; ++r)
    for (int c = 0; c < cols_; ++c)
      temp_image.at(c + r * cols_) = data_.at((c + (r * tmp_cols)) * scale);

  data_ = std::move(temp_image);
}

void Image::UpScale(int scale) {
  std::vector<int> tmp_image(data_.size() * scale * scale);
  for (int r = 0; r < rows_; ++r)
    for (int c = 0; c < cols_; ++c)
      for (int rcont = 0; rcont < scale; ++rcont)
        for (int ccont = 0; ccont < scale; ++ccont)
          tmp_image.at(c * scale + ccont +
                       (rcont + r * scale) * (cols_ * scale)) =
              data_.at(c + r * cols_);

  data_ = std::move(tmp_image);
  rows_ *= scale;
  cols_ *= scale;
}

}  // namespace igg