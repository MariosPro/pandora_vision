/*********************************************************************
*
* Software License Agreement (BSD License)
*
* Copyright (c) 2015, P.A.N.D.O.R.A. Team.
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
* * Redistributions of source code must retain the above copyright
* notice, this list of conditions and the following disclaimer.
* * Redistributions in binary form must reproduce the above
* copyright notice, this list of conditions and the following
* disclaimer in the documentation and/or other materials provided
* with the distribution.
* * Neither the name of the P.A.N.D.O.R.A. Team nor the names of its
* contributors may be used to endorse or promote products derived
* from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
* ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
* Authors:
*   Marios Protopapas <protopapas_marios@hotmail.com>
*   Kofinas Miltiadis <mkofinas@gmail.com>
*********************************************************************/

#include <vector>
#include <string>

#include "pandora_vision_victim/svm_classifier/rgb_svm_training.h"

namespace pandora_vision
{
  /**
   * @brief Constructor
   */
  RgbSvmTraining::RgbSvmTraining(const std::string& ns,
      int numFeatures, const std::string& datasetPath,
      const std::string& classifierType,
      const std::string& imageType)
      : SvmTraining(ns, numFeatures, datasetPath, classifierType, imageType)
  {
    std::string paramFile = packagePath_ + "/config/rgb_svm_training_params.yaml";
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

    featureExtraction_ = new RgbFeatureExtraction();
    std::cout << "Created RGB SVM Training Instance" << std::endl;
  }

  /**
   * @brief Destructor
   */
  RgbSvmTraining::~RgbSvmTraining()
  {
  }

  /**
   * @brief Function that implements the training for the subsystems
   * according to the given training sets. It applies SVM and extracts
   * a suitable model.
   * @return void
   */
  void RgbSvmTraining::trainSubSystem()
  {
    int numTrainingFiles = file_utilities::findNumberOfAnnotations(trainingAnnotationsFile_);
    int numTestFiles = file_utilities::findNumberOfAnnotations(testAnnotationsFile_);

    cv::Mat trainingFeaturesMat = cv::Mat::zeros(numTrainingFiles, numFeatures_, CV_64FC1);
    cv::Mat trainingLabelsMat = cv::Mat::zeros(numTrainingFiles, 1, CV_64FC1);
    cv::Mat testFeaturesMat = cv::Mat::zeros(numTestFiles, numFeatures_, CV_64FC1);
    cv::Mat testLabelsMat = cv::Mat::zeros(numTestFiles, 1, CV_64FC1);

    if (loadClassifierModel_ && file_utilities::exist(classifierFile_.c_str()))
    {
      SVM.load(classifierFile_.c_str());
    }
    else
    {
      if (file_utilities::exist(trainingFeaturesMatrixFile_.c_str()) == false ||
          trainingSetFeatureExtraction_)
      {
        std::cout << "Create necessary training matrix" << std::endl;
        std::string prefix = "training_";

        bool vocabularyNeeded = constructBagOfWordsVocabulary(trainingDirectory_,
            trainingAnnotationsFile_);

        constructFeaturesMatrix(trainingDirectory_, trainingAnnotationsFile_,
            &trainingFeaturesMat, &trainingLabelsMat);

        std::cout << "Normalize features" << std::endl;
        normalizeFeaturesAndSaveNormalizationParameters(&trainingFeaturesMat);

        trainingFeaturesMat.convertTo(trainingFeaturesMat, CV_32FC1);
        trainingLabelsMat.convertTo(trainingLabelsMat, CV_32FC1);

        file_utilities::saveFeaturesInFile(trainingFeaturesMat, trainingLabelsMat,
            prefix, trainingFeaturesMatrixFile_, trainingLabelsMatrixFile_,
            imageType_);

        if (vocabularyNeeded)
        {
          std::cout << "Save bag of words vocabulary" << std::endl;
          const std::string bagOfWordsFile = imageType_ + "bag_of_words.xml";
          const std::string bagOfWordsFilePath = filesDirectory_ + bagOfWordsFile;
          file_utilities::saveToFile(bagOfWordsFilePath, "bag_of_words",
              featureExtraction_->getBagOfWordsVocabulary());
        }
      }
      else
      {
        trainingFeaturesMat = file_utilities::loadFiles(
            trainingFeaturesMatrixFile_, "training_features_mat");
        trainingLabelsMat = file_utilities::loadFiles(
            trainingLabelsMatrixFile_, "training_labels_mat");
      }

      // Start Training Process
      std::cout << "Starting training process for the rgb images" << std::endl;
      if (vparams.autoTrain)
      {
        std::cout << "(SVM 'grid search' => may take some time!)" << std::endl;
        SVM.train_auto(trainingFeaturesMat, trainingLabelsMat, cv::Mat(), cv::Mat(),
                       vparams.params, 10, CvSVM::get_default_grid(CvSVM::C),
                       CvSVM::get_default_grid(CvSVM::GAMMA),
                       CvSVM::get_default_grid(CvSVM::P),
                       CvSVM::get_default_grid(CvSVM::NU),
                       CvSVM::get_default_grid(CvSVM::COEF),
                       CvSVM::get_default_grid(CvSVM::DEGREE),
                       true);

        vparams.params = SVM.get_params();
        std::cout << "Using optimal Parameters" << std::endl;
        std::cout << "degree=" << vparams.params.degree << std::endl;
        std::cout << "gamma=" << vparams.params.gamma << std::endl;
        std::cout << "coef0=" << vparams.params.coef0 << std::endl;
        std::cout << "C=" << vparams.params.C << std::endl;
        std::cout << "nu=" << vparams.params.nu << std::endl;
        std::cout << "p=" << vparams.params.p << std::endl;
      }
      else
      {
        SVM.train(trainingFeaturesMat, trainingLabelsMat, cv::Mat(), cv::Mat(), vparams.params);
      }
      SVM.save(classifierFile_.c_str());
      std::cout << "Finished training process" << std::endl;
    }
    if (file_utilities::exist(testFeaturesMatrixFile_.c_str()) == false ||
        testSetFeatureExtraction_)
    {
      std::cout << "Create necessary test matrix" << std::endl;
      std::string prefix = "test_";

      constructFeaturesMatrix(testDirectory_, testAnnotationsFile_,
          &testFeaturesMat, &testLabelsMat);

      loadNormalizationParametersAndNormalizeFeatures(&testFeaturesMat);

      testFeaturesMat.convertTo(testFeaturesMat, CV_32FC1);
      testLabelsMat.convertTo(testLabelsMat, CV_32FC1);

      file_utilities::saveFeaturesInFile(testFeaturesMat, testLabelsMat,
          prefix, testFeaturesMatrixFile_, testLabelsMatrixFile_, imageType_);
    }
    else
    {
      testFeaturesMat = file_utilities::loadFiles(
          testFeaturesMatrixFile_, "test_features_mat");
      testLabelsMat = file_utilities::loadFiles(
          testLabelsMatrixFile_, "test_labels_mat");
    }

    // uncomment to produce the platt probability
    // float prediction;
    // double A, B;
    // for (int ii = 0; ii < testFeaturesMat.rows; ii++)
    // {
      // prediction = SVM.predict(testFeaturesMat.row(ii), true);
      // results.at<double>(ii, 0)= prediction;
    // }
    // sigmoid_train(results, testLabelsMat, &A, &B);
    // std::cout << "A=" << A << std::endl;
    // std::cout << "B=" << B << std::endl;

    // uncomment for ONE_CLASS SVM
    // for (int ii = 0; ii < results.rows; ii++)
     // for (int jj = 0; jj < results.cols; jj++)
      // if(results.at<float>(ii, jj) == 0)
          // results.at<float>(ii, jj) = -1;
    cv::Mat results = cv::Mat::zeros(numTestFiles, 1, CV_64FC1);
    SVM.predict(testFeaturesMat, results);
    // std::cout << "results" << results.size() << std::endl << results <<std::endl <<std::endl;
    file_utilities::saveToFile(resultsFile_, "results", results);
    evaluate(results, testLabelsMat);
  }
}  // namespace pandora_vision

