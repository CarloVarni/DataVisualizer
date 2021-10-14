
#include <RootFileProfileMakerAlgorithm.hpp>

namespace Algorithm {

RootFileProfileMakerAlgorithm::RootFileProfileMakerAlgorithm(
    const std::string &name, const Config &cfg)
    : Core::BaseAlgorithm(name), m_cfg(cfg) {}

void RootFileProfileMakerAlgorithm::initialize() {
  MSG_INFO("Initializing " + name() + " ...");

  if (m_cfg.inputFile.empty())
    throw std::invalid_argument("Missing input file name for algorithm " +
                                name());

  if (m_cfg.output_collection.empty())
    throw std::invalid_argument(
        "Missing output collection name for algorithm " + name());
}

void RootFileProfileMakerAlgorithm::execute(Core::EventContext &context) {
  MSG_INFO("Executing " + name() + " ...");

  std::shared_ptr<TFile> file =
      std::make_shared<TFile>(m_cfg.inputFile.c_str(), "READ");

  std::shared_ptr<EventDataModel::ProfileObjectCollection> profile_collection =
      m_cfg.extractionFunction(file.get());

  std::shared_ptr<std::unordered_map<std::string, std::size_t>> profile_names =
      std::make_shared<std::unordered_map<std::string, std::size_t>>();

  profile_names->reserve(profile_collection->size());

  for (auto index(0); index < profile_collection->size(); index++) {
    const auto &object = profile_collection->at(index);
    (*profile_names.get())["prof_" + object.title()] = index;
  }

  if (not m_cfg.output_collection.empty()) {
    context.add("prof_" + m_cfg.output_collection,
                std::move(profile_collection));
    context.add("index_prof_" + m_cfg.output_collection,
                std::move(profile_names));
  }

  file->Close();
}

} // namespace Algorithm
