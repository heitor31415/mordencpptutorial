#pragma once

#include <string>
#include <vector>

namespace igg {

class Image {
 public:
  // Create empty image object
  Image();

  // Create image object with rows and cols
  Image(const int &rows, const int &cols);

  // Const refence to number of rows (rows_)
  const int &rows() const;
  // Const refence to number of columns (cols_)
  const int &cols() const;

  // non-const refence to pixel's value (0 based)
  // Examples of usage:
  // int val = image.at(row, col);
  // image.at(row, col) = 255;
  int &at(int row, int col);

  // io functions
  // Returns True if the Image was succesfully fille from the Pgm file
  bool FillFromPgm(const std::string &file_name);
  // Write image data into an ascii pgm file. No error checking
  void WriteToPgm(
      const std::string &file_name) const;  // **FUNCTION FROM io_tools lib has
                                            // a problem with Columns and rows!!

  // Return Historgram (vector with n bins)
  std::vector<float> ComputeHistogram(int bins) const;

  // SCALING FUNCTIONS
  // DownScale taking pixel every scale pixels
  void DownScale(int scale);
  // Upcase using nearest-neighbor interpolation
  void UpScale(int scale);

 private:
  int rows_ = 0;
  int cols_ = 0;
  int max_val_ = 255;
  std::vector<int> data_;
};

}  // namespace igg
