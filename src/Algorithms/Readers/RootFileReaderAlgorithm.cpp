
#include <MultiData.hpp>
#include <RootFileReaderAlgorithm.hpp>

#include <iostream>

namespace Algorithm {

RootFileReaderAlgorithm::RootFileReaderAlgorithm(
    const std::string &name,
    const Algorithm::RootFileReaderAlgorithm::Config &cfg)
    : BaseAlgorithm(name), m_cfg(cfg), m_tree(nullptr) {}

void RootFileReaderAlgorithm::initialize() {
  MSG_INFO("Initializing " + name() + " ...");

  if (m_cfg.fileName.empty())
    throw std::invalid_argument(
        "Input File Name has not been specified for algorithm " + name());

  if (m_cfg.treeName.empty())
    throw std::invalid_argument(
        "Input Tree Name has not been specified for algorithm " + name());

  if (m_cfg.dataCollectionName.empty())
    throw std::invalid_argument(
        "Output Collection Name has not been specified for algorithm " +
        name());

  if (m_cfg.outputMaskName.empty())
    throw std::invalid_argument(
        "Output Mask Name has not been specified for algorithm " + name());

  MSG_INFO("Opening ROOT file: " + m_cfg.fileName);
  m_file = std::make_unique<TFile>(m_cfg.fileName.c_str(), "READ");
  if (not m_file)
    throw std::runtime_error("Cannot open input ROOT file: " + m_cfg.fileName);

  MSG_INFO("Retrieving TTree object: " + m_cfg.treeName);
  m_tree = reinterpret_cast<TTree *>(m_file->Get(m_cfg.treeName.c_str()));
  if (not m_tree)
    throw std::runtime_error("Cannot retrieve TTree from ROOT file: " +
                             m_cfg.treeName);
}

void RootFileReaderAlgorithm::finalize() {
  MSG_INFO("Finalizing " + name() + " ...");

  if (m_file)
    m_file->Close();
}

} // namespace Algorithm
