#include "pngparser.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

#define ARG_SIZE 255

/* This filter iterates over the image and calculates the average value of the
 * color channels for every pixel This value is then written to all the channels
 * to get the grayscale representation of the image
 */
void filter_grayscale(struct image *img, void *weight_arr) {
  struct pixel(*image_data)[img->size_x] =
      (struct pixel(*)[img->size_x])img->px;
  double *weights = (double *)weight_arr;
  /* BUG!
   * This bug isn't graded.
   *
   * FIX: Initialize both variables to 0.
   */
  /* BUG!
   * This bug isn't graded.
   *
   * FIX: Initialize both variables to 0.
   */
  for (unsigned short i = 0; i < img->size_y; i++) {
    for (unsigned short j = 0; j < img->size_x; j++) {
      double luminosity = 0;

      luminosity += weights[0] * image_data[i][j].red;
      luminosity += weights[1] * image_data[i][j].green;
      luminosity += weights[2] * image_data[i][j].blue;

      image_data[i][j].red = (uint8_t)luminosity;
      image_data[i][j].green = (uint8_t)luminosity;
      image_data[i][j].blue = (uint8_t)luminosity;
    }
  }
}

/* This filter blurs an image. The larger the radius, the more noticeable the
 * blur.
 *
 * For every pixel we define a square of side 2*radius+1 centered around it.
 * The new value of the pixel is the average value of all pixels in the square.
 *
 * Pixels of the square which fall outside the image do not count towards the
 * average. They are ignored (e.g. 5x5 box will turn into a 3x3 box in the
 * corner).
 *
 */
void filter_blur(struct image *img, void *r) {
  struct pixel(*image_data)[img->size_x] =
      (struct pixel(*)[img->size_x])img->px;
  int radius = *((int *)r);
  if (radius < 0) {
    radius = 0;
  }

  
  struct pixel(*new_data)[img->size_x] =
      malloc(sizeof(struct pixel) * img->size_x * img->size_y);

  if (!new_data) {
    return;
  }

  for (long i = 0; i < img->size_y; i++) {
    for (long j = 0; j < img->size_x; j++) {

      unsigned red = 0, green = 0, blue = 0, alpha = 0;

      long y_min = i - radius;
      if (y_min < 0) {
        y_min = 0;
      }

      long y_max = i + radius;
      if (y_max >= img->size_y) {
        y_max = img->size_y - 1;
      }

      long x_min = j - radius;
      if (x_min < 0) {
        x_min = 0;
      }

      long x_max = j + radius;
      if (x_max >= img->size_x) {
        x_max = img->size_x - 1;
      }

      for (long y_offset = y_min; y_offset <= y_max; y_offset++) {
        for (long x_offset = x_min; x_offset <= x_max; x_offset++) {

          struct pixel current = image_data[y_offset][x_offset];

          red += current.red;
          blue += current.blue;
          green += current.green;
          alpha += current.alpha;
        }
      }


      int num_pixels = (2 * radius + 1) * (2 * radius + 1);
      red /= num_pixels;
      green /= num_pixels;
      blue /= num_pixels;
      alpha /= num_pixels;

      new_data[i][j].red = red;
      new_data[i][j].green = green;
      new_data[i][j].blue = blue;
      new_data[i][j].alpha = alpha;
      
    }
  }

  free(img->px);
  img->px = (struct pixel *)new_data; 
  return;
}

/* We allocate and return a pixel */
struct pixel *get_pixel() {
  struct pixel *px = malloc(sizeof(struct pixel));
  return px;
}

/* This filter just negates every color in the image */
void filter_negative(struct image *img, void *noarg) {
  struct pixel(*image_data)[img->size_x] =
      (struct pixel(*)[img->size_x])img->px;

  /* Iterate over all the pixels */
  for (long i = 0; i < img->size_y; i++) {
    for (long j = 0; j < img->size_x; j++) {

      struct pixel current = image_data[i][j];
      struct pixel *neg = get_pixel();

       if (!neg) {
        exit(1);
      }

      /* The negative is just the maximum minus the current value */
      neg->red = 255 - current.red;
      neg->green = 255 - current.green;
      neg->blue = 255 - current.blue;
      neg->alpha = current.alpha;

      /* Write it back */
      image_data[i][j] = *neg;
      free(neg);
    }
  }
}


/* Set the transparency of the picture to the value (0-255) passed as argument
 */
void filter_transparency(struct image *img, void *transparency) {

  struct pixel(*image_data)[img->size_x] =
      (struct pixel(*)[img->size_x])img->px;
  uint8_t local_alpha = *((uint8_t *)transparency);

  /* Iterate over all pixels */
  for (long i = 0; i < img->size_y; i++) {
    for (long j = 0; j < img->size_x; j++) {

      image_data[i][j].alpha = local_alpha;
    }
  }
}

/* This filter is used to detect edges by computing the gradient for each
 * pixel and comparing it to the threshold argument. When the gradient exceeds
 * the threshold, the pixel is replaced by black, otherwise white.
 * Alpha is unaffected.
 *
 * For each pixel and channel, the x-gradient and y-gradient are calculated
 * by using the following convolution matrices:
 *     Gx            Gy
 *  -1  0  +1     +1 +2 +1
 *  -2  0  +2      0  0  0
 *  -1  0  +1     -1 -2 -1
 * The convolution matrix are multiplied with the neighbouring pixels'
 * channel values. At edges, the indices are bounded.
 * Suppose the red channel values for the pixel and its neighbours are as
 * follows: 11 22 33 44 55 66 77 88 99 the x-gradient for red is: (-1*11 + 1*33
 * + -2*44 + 2*66 + -1*77 + 1*99).
 *
 * The net gradient for each channel = sqrt(g_x^2 + g_y^2)
 * For the pixel, the net gradient = sqrt(g_red^2 + g_green^2 + g_blue_2)
 */
void filter_edge_detect(struct image *img, void *threshold_arg) {
  struct pixel(*image_data)[img->size_x] =
      (struct pixel(*)[img->size_x])img->px;
  uint8_t threshold = *(uint8_t *)threshold_arg;
  double weights_x[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
  double weights_y[3][3] = {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}};

  /* Iterate over all pixels */
  for (long i = 0; i < img->size_y; i++) {
    for (long j = 0; j < img->size_x; j++) {

      //edge cases for convolution
      uint max_x = j+1;
      uint max_y = i+1;
      uint min_x = j-1;
      uint min_y = i-1;
      if (j == img->size_x) {
        max_x = img->size_x;
      }
      if (i == img->size_y) {
        max_y = img->size_y;
      }
      if (j == 0) {
        max_x = 0;
      }
      if (i == 0) {
        min_y = 0;
      }


     // Gx_red = -img_data[min_y][min_x].red + img_data[max_y][min_x].red -2*img_data[min_y][j].red + 2*img_data[max_y][j].red - 1*img_data[min_y][max_x].red + img_data[max_y][max_x].red;

    }
  }
}

/* The filter structure comprises the filter function, its arguments and the
 * image we want to process */
struct filter {
  void (*filter)(struct image *img, void *arg);
  void *arg;
  struct image *img;
};

void execute_filter(struct filter *fil) { fil->filter(fil->img, fil->arg); }

int __attribute__((weak)) main(int argc, char *argv[]) {
  struct filter fil;
  char input[ARG_SIZE];
  char output[ARG_SIZE];
  char command[ARG_SIZE];
  char arg[ARG_SIZE];
  int radius;
  struct pixel px;
  uint8_t alpha, depth, threshold;
  uint32_t key;
  struct image *img = NULL;
  double weights[] = {0.2125, 0.7154, 0.0721};

  /* Some filters take no arguments, while others have 1 */
  if (argc != 4 && argc != 5) {
    goto error_usage;
  }

  fil.filter = NULL;
  fil.arg = NULL;

  /* Copy arguments for easier reference */
  strncpy(input, argv[1], ARG_SIZE);
  strncpy(output, argv[2], ARG_SIZE);
  strncpy(command, argv[3], ARG_SIZE);

  /* If the filter takes an argument, copy it */
  if (argv[4]) {
    strncpy(arg, argv[4], ARG_SIZE);
  }

  /* Error when loading a png image */
  if (load_png(input, &img)) {
    printf(input);
    printf(" PNG file cannot be loaded\n");
    exit(1);
  }

  /* Set up the filter structure */
  fil.img = img;

  /* Decode the filter */
  if (!strcmp(command, "grayscale")) {
    fil.filter = filter_grayscale;
    fil.arg = weights;
  } else if (!strcmp(command, "negative")) {
    fil.arg = NULL;
    fil.filter = filter_negative;
  } else if (!strcmp(command, "blur")) {
    /* Bad filter radius will just be interpretted as 0 - no change to the image
     */
    radius = atoi(arg);
    fil.filter = filter_blur;
    fil.arg = &radius;
  } else if (!strcmp(command, "alpha")) {

    char *end_ptr;
    long tmp_alpha = strtol(arg, &end_ptr, 16);

    if (tmp_alpha < 0 || tmp_alpha > 255) {
      goto error_ustrcmpsage;
    } else {
      alpha = tmp_alpha;
    }

    if (*end_ptr) {
      goto error_usage;
    }

    fil.filter = filter_transparency;
    fil.arg = &alpha;
  } else if (!strcmp(command, "edge")) {
    char *end_ptr;
    threshold = strtol(arg, &end_ptr, 16);

    if (*end_ptr) {
      goto error_usage;
    }

    fil.filter = filter_edge_detect;
    fil.arg = &threshold;
  }

  /* Invalid filter check */
  if (fil.filter) {
    execute_filter(&fil);
  } else {
    goto error_filter;
  }

  store_png(output, img, NULL, 0);
  free(img->px);
  free(img);
  return 0;

error_filter:
  free(img->px);
  free(img);
error_usage:
  printf("Usage: %s input_image output_image filter_name [filter_arg]\n",
         argv[0]);
  printf("Filters:\n");
  printf("grayscale\n");
  printf("negative\n");
  printf("blur radius_arg\n");
  printf("alpha hex_alpha\n");
  printf("edge hex_threshold\n");
  return 1;
}
