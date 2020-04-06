#include <gtest/gtest.h>
#include <image.h>

#include <vector>

namespace {

int size_number = 500;       // tests with rows = cols
int rows = 203, cols = 305;  // rows != cols
unsigned int bins = 3;
int down_scale = 2, up_scale = 2;

TEST(Constructor, SpecifiedConstructor) {
  igg::Image image_specified(size_number, size_number);
  EXPECT_EQ(image_specified.cols(), image_specified.rows());
  EXPECT_EQ(image_specified.cols(), size_number);
  ASSERT_THROW(image_specified.at(size_number, size_number), std::out_of_range);
}

TEST(Constructor, DummyConstructor) {
  igg::Image empty_image;
  EXPECT_EQ(empty_image.rows(), 0);
  EXPECT_EQ(empty_image.cols(), 0);
  ASSERT_THROW(empty_image.at(0, 0), std::out_of_range);
}

TEST(IOTest, EmptyImage) {
  igg::Image io_image;  // Create empty image
  // Read from image
  EXPECT_TRUE(io_image.FillFromPgm("../data/lena.ascii.pgm"));
  EXPECT_EQ(io_image.rows(), 512);
  EXPECT_EQ(io_image.cols(), 512);

  // Testing a notFound image
  EXPECT_FALSE(io_image.FillFromPgm("notFound"));
  EXPECT_EQ(io_image.rows(), 512);
  EXPECT_EQ(io_image.cols(), 512);
}

TEST(IOTest, SpecifiedImage) {
  igg::Image io_image(rows, cols);  // Create empty image
  EXPECT_NO_THROW(io_image.WriteToPgm(
      "../data/empty.ascii.pgm"));  // write to empty.ascii.pgm
  // Read from image
  EXPECT_TRUE(io_image.FillFromPgm("../data/lena.ascii.pgm"));
  EXPECT_EQ(io_image.rows(), 512);
  EXPECT_EQ(io_image.cols(), 512);

  // Testing a notFound image
  EXPECT_FALSE(io_image.FillFromPgm("notFound"));
  EXPECT_EQ(io_image.rows(), 512);
  EXPECT_EQ(io_image.cols(), 512);

  // Reading smaller image
  ASSERT_TRUE(io_image.FillFromPgm("../data/empty.ascii.pgm"));
  EXPECT_EQ(io_image.rows(), rows);
  EXPECT_EQ(io_image.cols(), cols);

  // writing white lines
  EXPECT_NO_THROW(
      for (int r = 0; r < rows; r += 13) for (int c = 0; c < cols; ++c)
          io_image.at(r, c) = 255;);
  EXPECT_NO_THROW(io_image.WriteToPgm(
      "../data/lines.ascii.pgm"));  // write to lines.ascii.pgm

  // Number of rows and cols should not change
  EXPECT_EQ(io_image.rows(), rows);
  EXPECT_EQ(io_image.cols(), cols);
}

TEST(MethodTest, Histogram) {
  igg::Image io_image;  // Create empty image
  io_image.FillFromPgm("../data/lena.ascii.pgm");
  auto histogram_lena = io_image.ComputeHistogram(bins);
  EXPECT_EQ(histogram_lena.size(), bins);

  io_image.FillFromPgm("../data/empty.ascii.pgm");
  auto histogram_empty = io_image.ComputeHistogram(bins);
  EXPECT_EQ(histogram_empty.size(), bins);
  EXPECT_FLOAT_EQ(histogram_empty.front(), 1.);
  EXPECT_FLOAT_EQ(histogram_empty.at(1), 0.);
  EXPECT_FLOAT_EQ(histogram_empty.back(), 0.);

  // image with lines
  ASSERT_TRUE(io_image.FillFromPgm("../data/lines.ascii.pgm"));
  auto histogram_lines = io_image.ComputeHistogram(bins);
  EXPECT_LT(histogram_lines.front(), 1.);
  EXPECT_GT(histogram_lines.front(), 0.);
  EXPECT_GT(histogram_lines.back(), 0.);
  EXPECT_LT(histogram_lines.back(), 1.);
}

TEST(MethodTest, Downscale) {
  igg::Image io_image;  // Create empty image
  io_image.FillFromPgm("../data/lena.ascii.pgm");
  EXPECT_NO_THROW(io_image.DownScale(down_scale));
  EXPECT_EQ(io_image.rows(), 512 / down_scale);
  EXPECT_EQ(io_image.cols(), 512 / down_scale);
  EXPECT_NO_THROW(io_image.WriteToPgm("../data/down_lena.ascii.pgm"));
  io_image.FillFromPgm("../data/lines.ascii.pgm");
  EXPECT_NO_THROW(io_image.DownScale(down_scale));
  EXPECT_EQ(io_image.rows(), rows / down_scale);
  EXPECT_EQ(io_image.cols(), cols / down_scale);
  EXPECT_NO_THROW(io_image.WriteToPgm("../data/down_lines.ascii.pgm"));
}

TEST(MethodTest, Upscale) {
  igg::Image io_image;  // Create empty image
  io_image.FillFromPgm("../data/lena.ascii.pgm");
  EXPECT_NO_THROW(io_image.UpScale(up_scale));
  EXPECT_EQ(io_image.rows(), 512 * up_scale);
  EXPECT_EQ(io_image.cols(), 512 * up_scale);
  EXPECT_NO_THROW(io_image.WriteToPgm("../data/up_lena.ascii.pgm"));
  io_image.FillFromPgm("../data/lines.ascii.pgm");
  EXPECT_NO_THROW(io_image.UpScale(up_scale));
  EXPECT_EQ(io_image.rows(), rows * up_scale);
  EXPECT_EQ(io_image.cols(), cols * up_scale);
  EXPECT_NO_THROW(io_image.WriteToPgm("../data/up_lines.ascii.pgm"));
}

}  // namespace