/*********************************************************************
*
* Software License Agreement (BSD License)
*
*  Copyright (c) 2015, P.A.N.D.O.R.A. Team.
*  All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*   * Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   * Redistributions in binary form must reproduce the above
*     copyright notice, this list of conditions and the following
*     disclaimer in the documentation and/or other materials provided
*     with the distribution.
*   * Neither the name of the P.A.N.D.O.R.A. Team nor the names of its
*     contributors may be used to endorse or promote products derived
*     from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
*  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
*  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
*  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
*  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
*  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
*  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*  POSSIBILITY OF SUCH DAMAGE.
*
* Authors:
*   Marios Protopapas
*   Kofinas Miltiadis <mkofinas@gmail.com>
*********************************************************************/

#include "pandora_vision_victim/svm_classifier/svm_training.h"

namespace pandora_vision
{
  /**
   * @brief Constructor
   */
  SvmTraining::SvmTraining(const std::string& ns,
      int _num_feat,
      const std::string& datasetPath) :
      _nh(ns), vparams(ns)
  {
    path_to_samples = datasetPath;
    num_feat = _num_feat;
    params.svm_type = CvSVM::C_SVC;
    params.kernel_type = CvSVM::RBF; //CvSVM::RBF, CvSVM::LINEAR ...
    params.degree = 1; // for poly
    params.gamma = 5.0625000000000009e-01; // for poly/rbf/sigmoid
    params.coef0 = 0; // for poly/sigmoid
    //~ CvParamGrid CvParamGrid_C(pow(2.0,-5), pow(2.0,15), pow(2.0,2));
    //~ CvParamGrid CvParamGrid_gamma(pow(2.0,-20), pow(2.0,3), pow(2.0,2));
    //~ if (!CvParamGrid_C.check() || !CvParamGrid_gamma.check())
      //~ std::cout << "The grid is NOT VALID." << std::endl;
    params.C = 3.1250000000000000e+02; // for CV_SVM_C_SVC, CV_SVM_EPS_SVR and CV_SVM_NU_SVR
    params.nu = 0.3; // for CV_SVM_NU_SVC, CV_SVM_ONE_CLASS, and CV_SVM_NU_SVR
    params.p = 0.0; // for CV_SVM_EPS_SVR
    params.class_weights = NULL; // for CV_SVM_C_SVC
    params.term_crit.type = CV_TERMCRIT_ITER+CV_TERMCRIT_EPS;
    params.term_crit.max_iter = 10000;
    params.term_crit.epsilon = 1e-6;

    package_path = ros::package::getPath("pandora_vision_victim");
    ROS_INFO("[victim_node] : Created Svm training instance");

    doFeatureExtraction_ = true;
    featureExtractionUtilities_ = new FeatureExtractionUtilities();
    featureExtraction_ = new FeatureExtraction();
  }

  /**
   * @brief Destructor
   */
  SvmTraining::~SvmTraining()
  {
    ROS_DEBUG("[victim_node] : Destroying Svm training instance");
  }

  /**
   * @brief This function constructs the features matrix, i.e. the feature
   * vectors of a set of images.
   * @param directory [const boost::filesystem::path&] The directory that
   * contains the set of images for the feature extraction.
   * @param featuresMat [cv::Mat*] The features matrix.
   * @param labelsMat [cv::Mat*] The matrix that contains the class attributes
   * for the processed set of images.
   * @return void
   */
  void SvmTraining::constructFeaturesMatrix(
      const boost::filesystem::path& directory,
      cv::Mat* featuresMat, cv::Mat* labelsMat)
  {
    featureExtraction_->constructFeaturesMatrix(directory,
        featuresMat, labelsMat);
  }

  /**
   * @brief This function evaluates the classifier model, based on the predicted
   * and the actual class attributes.
   * @param predicted [const cv::Mat&] The predicted class attributes.
   * @param actual [const cv::Mat&] The actual class attributes.
   * @return void
   */
  void SvmTraining::evaluate(const cv::Mat& predicted, const cv::Mat& actual)
  {
    assert(predicted.rows == actual.rows);
    int truePositives = 0;
    int falsePositives = 0;
    int trueNegatives = 0;
    int falseNegatives = 0;
    for (int ii = 0; ii < actual.rows; ii++)
    {
      float p = predicted.at<float>(ii, 0);
      float a = actual.at<float>(ii, 0);

      if (p >= 0.0 && a >= 0.0)
        truePositives++;
      else if (p <= 0.0 && a <= 0.0)
        trueNegatives++;
      else if (p >= 0.0 && a <= 0.0)
        falsePositives++;
      else if (p <= 0.0 && a >= 0.0)
        falseNegatives++;
    }
    accuracy_ = static_cast<float>(truePositives + trueNegatives) /
              (truePositives + trueNegatives + falsePositives + falseNegatives);
    precision_ = static_cast<float>(truePositives) /
              (truePositives + falsePositives);
    recall_ = static_cast<float>(truePositives) /
              (truePositives + falseNegatives);
    fmeasure_ = (2.0 * truePositives) /
              (2.0 * truePositives + falseNegatives + falsePositives);

    std::cout << "True Positives = " << truePositives << std::endl;
    std::cout << "True Negatives = " << trueNegatives << std::endl;
    std::cout << "False Positives = " << falsePositives << std::endl;
    std::cout << "False Negatives = " << falseNegatives << std::endl;

    std::cout << "SVM Accuracy = " << accuracy_ << std::endl;
    std::cout << "SVM Precision = " << precision_ << std::endl;
    std::cout << "SVM Recall = " << recall_ << std::endl;
    std::cout << "SVM F-Measure = " << fmeasure_ << std::endl;
  }

  // Platt's binary SVM Probablistic Output: an improvement from Lin et al.
  /**
   * @brief Function that computes the vectors A,B necessary for the
   * computation of the probablistic output of the SVM bases on Platt's binary
   * SVM probablistic Output
   * @param dec_values [cv::Mat] the distance from the hyperplane of the
   * predicted results of the given test dataset
   * @param labels [cv::Mat] the true labels of the dataset
   * @param A [double*] the vector A to be computed
   * @param B [double*] the vector B to be computed
   * @return void
   */
  void SvmTraining::sigmoid_train(cv::Mat dec_values, cv::Mat labels,
                                  double* A, double* B)
  {
    double prior1 = 0, prior0 = 0;

    for (int ii = 0; ii < dec_values.rows; ii++)
      if (labels.at<double>(ii, 0) > 0)
        prior1 += 1;
      else
        prior0+= 1;

    int max_iter = 100;// Maximal number of iterations
    double min_step = 1e-10;// Minimal step taken in line search
    double sigma = 1e-12;// For numerically strict PD of Hessian
    double eps = 1e-5;
    double hiTarget = (prior1 + 1.0) / (prior1 + 2.0);
    double loTarget = 1 / (prior0 + 2.0);
    double* t = new double[labels.rows];
    double fApB, p, q, h11, h22, h21, g1, g2, det, dA, dB, gd, stepsize;
    double newA, newB, newf, d1, d2, Avector, Bvector;
    int iter;

    // Initial Point and Initial Fun Value
    Avector = 0.0;
    Bvector = log((prior0 + 1.0) / (prior1 + 1.0));
    double fval = 0.0;

    for (int ii = 0; ii <labels.rows; ii++)
    {
      if (labels.at<double>(ii, 0) > 0)
        t[ii] = hiTarget;
      else
        t[ii]=loTarget;
      fApB = dec_values.at<double>(ii, 0) * Avector + Bvector;
      if (fApB >= 0)
        fval += t[ii] * fApB + log(1 + exp(-fApB));
      else
        fval += (t[ii] - 1) * fApB + log(1 + exp(fApB));
    }
    for (iter = 0; iter < max_iter; iter++)
    {
      // Update Gradient and Hessian (use H' = H + sigma I)
      h11 = sigma; // numerically ensures strict PD
      h22 = sigma;
      h21 = 0.0;
      g1 = 0.0;
      g2 = 0.0;
      for (int ii = 0; ii < dec_values.rows; ii++)
      {
        fApB = dec_values.at<double>(ii, 0) * Avector + Bvector;
        if (fApB >= 0)
        {
          p = exp(-fApB) / (1.0 + exp(-fApB));
          q = 1.0 / (1.0 + exp(-fApB));
        }
        else
        {
          p = 1.0 / (1.0 + exp(fApB));
          q = exp(fApB) / (1.0 + exp(fApB));
        }
        d2 = p * q;
        h11 += dec_values.at<double>(ii, 0) * dec_values.at<double>(ii, 0) * d2;
        h22 += d2;
        h21 += dec_values.at<double>(ii, 0) * d2;
        d1 = t[ii] - p;
        g1 += dec_values.at<double>(ii, 0) * d1;
        g2 += d1;
      }

      // Stopping Criteria
      if (fabs(g1) < eps && fabs(g2) < eps)
        break;

      // Finding Newton direction: -inv(H') * g
      det= h11 * h22 - h21 * h21;
      dA =- (h22 * g1 - h21 * g2) / det;
      dB=-(-h21 * g1 + h11 * g2) / det;
      gd = g1 * dA + g2 * dB;


      stepsize = 1;// Line Search
      while (stepsize >= min_step)
      {
        newA = Avector + stepsize * dA;
        newB = Bvector + stepsize * dB;

        // New function value
        newf = 0.0;
        for (int ii = 0; ii < labels.rows; ii++)
        {
          fApB = dec_values.at<double>(ii, 0) * newA + newB;
          if (fApB >= 0)
            newf += t[ii] * fApB + log(1 + exp(-fApB));
          else
            newf += (t[ii] - 1) * fApB +log(1 + exp(fApB));
        }
        // Check sufficient decrease
        if (newf < fval + 0.0001 * stepsize * gd)
        {
          Avector = newA;
          Bvector = newB;
          fval = newf;
          break;
        }
        else
          stepsize = stepsize / 2.0;
      }

      if (stepsize < min_step)
      {
        std::cout << "Line search fails in two-class probability estimates" << std::endl;
        break;
      }
    }

    if (iter >= max_iter)
      std::cout << "Reaching maximal iterations in two-class probability estimates" << std::endl;
    free(t);
    *A = Avector;
    *B = Bvector;
  }
}// namespace pandora_vision

