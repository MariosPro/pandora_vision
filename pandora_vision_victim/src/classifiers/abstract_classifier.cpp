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
*   Kofinas Miltiadis <mkofinas@gmail.com>
*********************************************************************/

#include <vector>
#include <string>

#include "pandora_vision_victim/utilities/file_utilities.h"
#include "pandora_vision_victim/feature_extractors/rgb_feature_extraction.h"
#include "pandora_vision_victim/feature_extractors/depth_feature_extraction.h"
#include "pandora_vision_victim/classifiers/abstract_classifier.h"

namespace pandora_vision
{
  /**
   * @brief Constructor. Initialize member variables.
   */
  AbstractClassifier::AbstractClassifier(const std::string& ns,
      int numFeatures, const std::string& datasetPath,
      const std::string& classifierType, const std::string& imageType) :
      nh_(ns)
  {
    vparams.configVictim(nh_);

    datasetPath_ = datasetPath;
    numFeatures_ = numFeatures;
    imageType_ = imageType;
    classifierType_ = classifierType;

    packagePath_ = ros::package::getPath("pandora_vision_victim");
    ROS_INFO("[victim_node] : Created Abstract Classifier instance");

    // featureExtractionUtilities_ = new FeatureExtractionUtilities();

    filesDirectory_ = packagePath_ + "/data/";

    const std::string filePrefix = filesDirectory_ + imageType_ + "_";
    trainingFeaturesMatrixFile_ = filePrefix + "training_features_matrix.xml";
    testFeaturesMatrixFile_ = filePrefix + "test_features_matrix.xml";
    trainingLabelsMatrixFile_ = filePrefix + "training_labels_matrix.xml";
    testLabelsMatrixFile_ = filePrefix + "test_labels_matrix.xml";
    resultsFile_ = filePrefix + "results.xml";
    classifierFile_ = filePrefix + "classifier.xml";

    const std::string trainingDatasetPath = datasetPath_ + "/Training_Images";
    boost::filesystem::path trainingDirectory(trainingDatasetPath);
    trainingDirectory_ = trainingDirectory;

    trainingAnnotationsFile_ = filePrefix + "training_annotations.txt";
    int numTrainingFiles = file_utilities::findNumberOfAnnotations(trainingAnnotationsFile_);

    const std::string testDatasetPath = datasetPath_ + "/Test_Images";
    boost::filesystem::path testDirectory(testDatasetPath);
    testDirectory_ = testDirectory;

    testAnnotationsFile_ = filePrefix + "test_annotations.txt";
    int numTestFiles = file_utilities::findNumberOfAnnotations(testAnnotationsFile_);

    std::string paramFile = packagePath_ + "/config/" + imageType + "_" + classifierType + "_training_params.yaml";
    cv::FileStorage fs(paramFile, cv::FileStorage::READ);
    fs.open(paramFile, cv::FileStorage::READ);

    std::string trainingSetExtraction = fs["training_set_feature_extraction"];
    std::string testSetExtraction = fs["test_set_feature_extraction"];
    std::string loadModel = fs["load_classifier_model"];
    std::string pcaAnalysis = fs["do_pca_analysis"];

    trainingSetFeatureExtraction_ = trainingSetExtraction.compare("true") == 0;
    testSetFeatureExtraction_ = testSetExtraction.compare("true") == 0;
    loadClassifierModel_ = loadModel.compare("true") == 0;
    doPcaAnalysis_ = pcaAnalysis.compare("true") == 0;
    typeOfNormalization_ = static_cast<int>(fs["type_of_normalization"]);

    fs.release();

    if (imageType_.compare("rgb") == 0)
      featureExtraction_ = new RgbFeatureExtraction();
    else if (imageType_.compare("depth") == 0)
      featureExtraction_ = new DepthFeatureExtraction();
    else
    {
      ROS_ERROR("Invalid image type provided!");
      ROS_ERROR("The system will now shut down!");
      ros::shutdown();
    }


    ROS_INFO("Created Abstract Classifier Instance!");
  }

  /**
   * @brief Destructor
   */
  AbstractClassifier::~AbstractClassifier()
  {
    ROS_DEBUG("[victim_node] : Destroying Abstract Classifier instance");
  }

  /**
   * @brief This function normalizes the features and saves normalization
   * parameters in a file.
   * @param featuresMatrix [cv::Mat*] The features matrix to be normalized.
   */
  void AbstractClassifier::normalizeFeaturesAndSaveNormalizationParameters(
      cv::Mat* featuresMatrix)
  {
    if (typeOfNormalization_ == 0)
    {
      return;
    }

    std::string normalizationParamOne;
    std::string normalizationParamTwo;

    std::string normalizationParamOneTag;
    std::string normalizationParamTwoTag;

    std::vector<double> normalizationParamOneVector;
    std::vector<double> normalizationParamTwoVector;

    if (typeOfNormalization_ == 1)
    {
      double newMin = -1.0;
      double newMax = 1.0;
      featureExtractionUtilities_->findMinMaxParameters(newMax, newMin,
          featuresMatrix, &normalizationParamOneVector,
          &normalizationParamTwoVector);

      normalizationParamOneTag = "min";
      normalizationParamTwoTag = "max";

      normalizationParamOne = imageType_ + "_min_values";
      normalizationParamTwo = imageType_ + "_max_values";
    }
    else
    {
      featureExtractionUtilities_->findZScoreParameters(featuresMatrix,
          &normalizationParamOneVector, &normalizationParamTwoVector);

      normalizationParamOneTag = "mean";
      normalizationParamTwoTag = "std_dev";

      normalizationParamOne = imageType_ + "_mean_values.xml";
      normalizationParamTwo = imageType_ + "_standard_deviation_values.xml";
    }

    std::string normalizationParamOnePath = filesDirectory_ + normalizationParamOne;
    std::string normalizationParamTwoPath = filesDirectory_ + normalizationParamTwo;

    file_utilities::saveToFile(normalizationParamOnePath,
        normalizationParamOneTag,
        cv::Mat(normalizationParamOneVector));
    file_utilities::saveToFile(normalizationParamTwoPath,
        normalizationParamTwoTag,
        cv::Mat(normalizationParamTwoVector));
  }

  /**
   * @brief This function loads normalization parameters and normalizes the
   * input features matrix.
   * @param featuresMatrix [cv::Mat*] The features matrix to be normalized.
   */
  void AbstractClassifier::loadNormalizationParametersAndNormalizeFeatures(
      cv::Mat* featuresMatrix)
  {
    if (typeOfNormalization_ == 0)
    {
      return;
    }

    std::string normalizationParamOne;
    std::string normalizationParamOnePath;
    std::string normalizationParamTwo;
    std::string normalizationParamTwoPath;

    std::string normalizationParamOneTag;
    std::string normalizationParamTwoTag;

    std::vector<double> normalizationParamOneVector;
    std::vector<double> normalizationParamTwoVector;

    if (typeOfNormalization_ == 1)
    {
      double newMax = 1.0;
      double newMin = -1.0;

      normalizationParamOne = imageType_ + "_min_values.xml";
      normalizationParamOnePath = filesDirectory_ + normalizationParamOne;
      normalizationParamTwo = imageType_ + "_max_values.xml";
      normalizationParamTwoPath = filesDirectory_ + normalizationParamTwo;

      normalizationParamOneTag = "min";
      normalizationParamTwoTag = "max";
      normalizationParamOneVector = file_utilities::loadFiles(
          normalizationParamOnePath,
          normalizationParamOneTag);
      normalizationParamTwoVector = file_utilities::loadFiles(
          normalizationParamTwoPath,
          normalizationParamTwoTag);
      featureExtractionUtilities_->performMinMaxNormalization(newMax, newMin,
          featuresMatrix, normalizationParamOneVector,
          normalizationParamTwoVector);
    }
    else
    {
      normalizationParamOne = imageType_ + "_mean_values.xml";
      normalizationParamOnePath = filesDirectory_ + normalizationParamOne;
      normalizationParamTwo = imageType_ + "_standard_deviation_values.xml";
      normalizationParamTwoPath = filesDirectory_ + normalizationParamTwo;

      normalizationParamOneTag = "mean";
      normalizationParamTwoTag = "std_dev";
      normalizationParamOneVector = file_utilities::loadFiles(
          normalizationParamOnePath,
          normalizationParamOneTag);
      normalizationParamTwoVector = file_utilities::loadFiles(
          normalizationParamTwoPath,
          normalizationParamTwoTag);
      featureExtractionUtilities_->performZScoreNormalization(
          featuresMatrix, normalizationParamOneVector,
          normalizationParamTwoVector);
    }
  }

  /**
   * @brief
   */
  bool AbstractClassifier::constructBagOfWordsVocabulary(
      const boost::filesystem::path& directory,
      const std::string& annotationsFile)
  {
    return featureExtraction_->constructBagOfWordsVocabulary(directory,
        annotationsFile);
  }

  /**
   * @brief This function constructs the features matrix, i.e. the feature
   * vectors of a set of images.
   * @param directory [const boost::filesystem::path&] The directory that
   * contains the set of images for the feature extraction.
   * @param annotationsFile [const std::string&] The name of the file that
   * contains the class attributes of the images to be processed.
   * @param featuresMat [cv::Mat*] The features matrix.
   * @param labelsMat [cv::Mat*] The matrix that contains the class attributes
   * for the processed set of images.
   * @return void
   */
  void AbstractClassifier::constructFeaturesMatrix(
      const boost::filesystem::path& directory,
      const std::string& annotationsFile,
      cv::Mat* featuresMat, cv::Mat* labelsMat)
  {
    featureExtraction_->constructFeaturesMatrix(directory,
        annotationsFile, featuresMat, labelsMat);
  }

  void AbstractClassifier::trainSubSystem()
  {
  }

  /**
   * @brief This function evaluates the classifier model, based on the predicted
   * and the actual class attributes.
   * @param predicted [const cv::Mat&] The predicted class attributes.
   * @param actual [const cv::Mat&] The actual class attributes.
   * @return void
   */
  void AbstractClassifier::evaluate(const cv::Mat& predicted, const cv::Mat& actual)
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

    std::cout << classifierType_ << "Accuracy = " << accuracy_ << std::endl;
    std::cout << classifierType_ << "Precision = " << precision_ << std::endl;
    std::cout << classifierType_ << "Recall = " << recall_ << std::endl;
    std::cout << classifierType_ << "F-Measure = " << fmeasure_ << std::endl;
  }

  // Platt's binary Probablistic Output: an improvement from Lin et al.
  /**
   * @brief Function that computes the vectors A,B necessary for the
   * computation of the probablistic output of the Classifier bases on Platt's binary
   * probablistic Output
   * @param dec_values [cv::Mat] the distance from the hyperplane of the
   * predicted results of the given test dataset
   * @param labels [cv::Mat] the true labels of the dataset
   * @param A [double*] the vector A to be computed
   * @param B [double*] the vector B to be computed
   * @return void
   */
  void AbstractClassifier::sigmoid_train(cv::Mat dec_values, cv::Mat labels,
                                  double* A, double* B)
  {
    double prior1 = 0, prior0 = 0;

    for (int ii = 0; ii < dec_values.rows; ii++)
      if (labels.at<double>(ii, 0) > 0)
        prior1 += 1;
      else
        prior0+= 1;

    int max_iter = 100;  // Maximal number of iterations
    double min_step = 1e-10;  // Minimal step taken in line search
    double sigma = 1e-12;  // For numerically strict PD of Hessian
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
      h11 = sigma;  // numerically ensures strict PD
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


      stepsize = 1;  // Line Search
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
}  // namespace pandora_vision

