#include <string>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;

/*

    b                                     c
    +--------------+----------------------+
    |              |                      |
    |              |                      |
    |              | y1                   |
    |              |                      |
    |              |                      |
    |     x1       |          x2          |
 t1 +--------------p----------------------+ t2
    |              |                      |
    |              |                      |
    |              |                      |
    |              | y2                   |
    |              |                      |
    |              |                      |
    |              |                      |
    +--------------+----------------------+
    a                                     d
*/
double billinear( double x1, double x2, double y1, double y2, double xp,
                  double yp, double fa, double fb, double fc, double fd)
{
  double H = y1 + y2;
  double W = x1 + x2;

  double t1 = fb * y2 / H + fa * y1 / H;
  double t2 = fc * y2 / H + fd * y1 / H;

  return t1 * x2 / W + t2 * x1 / W;
}

Mat resize(Mat const& inp, int newH, int newW)
{
  int w_inp = inp.cols;
  int h_inp = inp.rows;
  int h_ret = newH;
  int w_ret = newW;

  Mat ret;

  if (inp.type() == CV_8UC1)
    ret = Mat(h_ret, w_ret, CV_8UC1, cv::Scalar(0));
  else
    ret = Mat(h_ret, w_ret, CV_8UC3, cv::Scalar(0, 0, 0));

  double rx = (double(w_inp) - 1.0) / (double(w_ret) - 1.0);
  double ry = (double(h_inp) - 1.0) / (double(h_ret) - 1.0);

  for (int y = 0; y < h_ret; ++y)
  {
    for (int x = 0; x < w_ret; ++x)
    {
      double xp = ((double) x) * rx;
      double yp = ((double) y) * ry;

      double x1 = xp - floor(xp);
      double x2 = ceil(xp) - xp;
      double y1 = yp - floor(yp);
      double y2 = ceil(yp) - yp;

      int ya = floor(yp);
      int xa = floor(xp);

      Vec3b &retAt = ret.at<Vec3b>(y, x);
      const Vec3b &aAt = inp.at<Vec3b>(ya, xa);
      const Vec3b &bAt = inp.at<Vec3b>(ya + 1, xa);
      const Vec3b &cAt = inp.at<Vec3b>(ya + 1, xa + 1);
      const Vec3b &dAt = inp.at<Vec3b>(ya, xa + 1);

      for(int k = 0; k < ret.channels(); k++)
      {
        retAt.val[k] = billinear(x1, x2, y1, y2, xp, yp, aAt[k], bAt[k], cAt[k], dAt[k]);
      }
    }
  }
  return ret;
}


int main()
{
    std::string image_path = "lenna.png";
    Mat inp = imread(image_path, IMREAD_COLOR);
    Mat resized = resize(inp, 350, 700);
    imshow("original", inp);
    imshow("resized", resized);
    int k = waitKey(0);
    return 0;
}

