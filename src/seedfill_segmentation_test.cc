/*
 * seedfill_segmentation_test.cc
 *
 *  Created on: Oct 28, 2020
 *      Author: amyznikov
 *
 *  Test for C++/OpenCV implementation of Connected component labeling algorithm from paper by M. Emre Celebi,
 *    "A Simple and Efficient Algorithm for Connected Component Labeling in Color Images".
 *
 *  https://ui.adsabs.harvard.edu/abs/2012SPIE.8295E..1HC/abstract
 *
 */

#include "seedfill_segmentation.h"
//////////////////////////////////////////////////////////////////////////////////////


// REALTIME ms
static double c_get_realtime_ms(void)
{
  struct timespec t;
  clock_gettime(CLOCK_REALTIME, &t);
  return (double) (t.tv_sec * 1000 + t.tv_nsec / 1e6);
}



int main(int argc, char *argv[])
{
  std::string input_file_name;
  std::string output_file_name;
  double threshold = 20;

  cv::Mat input_image;
  cv::Mat1i labels;
  int max_label = 0;

  double t0, t1;

  for ( int i = 1; i < argc; ++i ) {

    if ( strcmp(argv[i], "--help")  == 0 ) {
      fprintf(stdout,
          "Usage:\n"
          "    seedfill_segmentation_test [OPTIONS] <input-image-file-name> [-o <output-image-file-name>]\n"
          "OPTIONS:\n"
          "   -t <threshold>\n"
          "\n");
      return 0;
    }

    if ( strcmp(argv[i], "-o")  == 0 ) {
      if ( ++i >= argc ) {
        fprintf(stderr, "Command line error: missing output file name after %s argument\n", argv[i - 1]);
        return 1;
      }
      output_file_name = argv[i];
    }
    else if ( strcmp(argv[i], "-t")  == 0 ) {
      if ( ++i >= argc ) {
        fprintf(stderr, "Command line error: missing threshold parameter after %s argument\n", argv[i - 1]);
        return 1;
      }

      if ( sscanf(argv[i], "%lf", &threshold)  != 1 ) {
        fprintf(stderr, "Command line error: invalid threshold parameter '%s' specified after %s\n", argv[i], argv[i - 1]);
        return 1;
      }
    }

    else if ( input_file_name.empty() ) {
      input_file_name = argv[i];
    }
    else {
      fprintf(stderr, "Command line error: invalid argument %s\n", argv[i]);
      return 1;
    }
  }

  if ( input_file_name.empty() ) {
    fprintf(stderr, "No input image specified\n");
    return 1;
  }

  if ( !(input_image = cv::imread(input_file_name, cv::IMREAD_UNCHANGED)).data ) {
    fprintf(stderr, "cv::imread('%s') fails\n", input_file_name.c_str());
    return 1;
  }


  fprintf(stderr, "Input image: %dx%dx%d depth=%d\n",
      input_image.cols, input_image.rows,
      input_image.channels(),
      input_image.depth());





  t0 = c_get_realtime_ms();

  max_label = seedfill_segmentation(input_image,
      labels,
      threshold);

  t1 = c_get_realtime_ms();

  fprintf(stderr, "max_label=%d in %g ms\n", max_label, t1 - t0);



  //
  // colorize segments in random colors
  //
  std::vector<cv::Vec3b> colors;
  cv::Mat3b colorized_labels;
  cv::RNG rng;

  colors.resize(max_label);
  rng.fill(colors, cv::RNG::UNIFORM, 16, 255);

  colorized_labels.create(labels.size());
  colorized_labels.setTo(0);
  for ( int y = 0; y < colorized_labels.rows; ++y ) {
    for ( int x = 0; x < colorized_labels.cols; ++x ) {
      colorized_labels[y][x] = colors[labels[y][x] - 1];
    }
  }

  if ( output_file_name.empty() ) {
    output_file_name = "colorized_labels.png";
  }

  //
  // Save colorized labels
  //
  if ( !cv::imwrite(output_file_name, colorized_labels) ) {
    fprintf(stderr, "cv::imwrite('%s') fails\n", output_file_name.c_str());
    return 1;
  }


  fprintf(stderr, "Colorized segments saved to %s\n",
      output_file_name.c_str());


  return 0;
}


