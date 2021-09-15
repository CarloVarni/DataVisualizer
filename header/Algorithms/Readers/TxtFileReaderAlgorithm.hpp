
#ifndef TXTFILE_READER_ALGORITHM_H
#define TXTFILE_READER_ALGORITHM_H 1

#include <BaseAlgorithm.hpp>
#include <Data.hpp>
#include <MultiData.hpp>

#include <string>
#include <vector>
#include <functional>
#include <fstream>

namespace Algorithm {

  template<typename data_t>
  class TxtFileReaderAlgorithm
    : public Core::BaseAlgorithm {
  public:
    using data_type = data_t;

    static_assert(std::is_same<data_t, EventDataModel::DataObjectCollection>::value or
		  std::is_same<data_t, EventDataModel::MultiDataObjectCollection>::value,
		  "TxtFileReaderAlgorithm only support DataObjectCollection and MultiDataObjectCollection output format");
    
    struct Config {
      std::string inputFile = "";
      std::string outputCollection = "";
      std::string outputMask = "";
      std::function<void(std::ifstream& file,
			 std::shared_ptr<data_t>& data_collection)> extraction_function;
    };

    TxtFileReaderAlgorithm() = delete;
    TxtFileReaderAlgorithm(const std::string& name,
			   const Config& cfg);
    TxtFileReaderAlgorithm(const TxtFileReaderAlgorithm&) = delete;
    TxtFileReaderAlgorithm& operator=(const TxtFileReaderAlgorithm&) = delete;
    virtual ~TxtFileReaderAlgorithm() = default;

    virtual void initialize() override;
    virtual void execute(Core::EventContext&) override;
  
  private:
    const Config m_cfg;
  };

  /* ============================================================== */
  
  template<typename data_t>
  TxtFileReaderAlgorithm<data_t>::TxtFileReaderAlgorithm(const std::string& name,
							 const Config& cfg)
    : Core::BaseAlgorithm(name),
      m_cfg(cfg)
  {}

  template<typename data_t>
  void
  TxtFileReaderAlgorithm<data_t>::initialize()
  {
    MSG_INFO("Initializing " + name() + " ...");

    if (m_cfg.inputFile.empty())
      throw std::invalid_argument("Missing input file for algorithm " + name());

    if (m_cfg.outputCollection.empty())
      throw std::invalid_argument("Missing output collection name for algorithm " + name());

    if (m_cfg.outputMask.empty())
      throw std::invalid_argument("Missing output mask name for algorithm " + name());
  }

  template<typename data_t>
  void
  TxtFileReaderAlgorithm<data_t>::execute(Core::EventContext& context)
  {
    MSG_INFO("Executing " + name() + " ...");

    std::shared_ptr<data_t> data_collection =
      std::make_shared<data_t>();

    std::ifstream input_file;
    input_file.open(m_cfg.inputFile);

    if ( not input_file.is_open()) {
      MSG_FATAL("Input file has not been found!");
    }
    
    m_cfg.extraction_function(input_file, data_collection);
    MSG_INFO("Retrieved " + std::to_string(data_collection->size()) + " data objects");
    input_file.close();

    // create mask
    std::shared_ptr<std::vector<bool>> data_mask =
      std::make_shared<std::vector<bool>>(data_collection->size(), true);
    
    context.add(m_cfg.outputCollection, std::move(data_collection));
    context.add(m_cfg.outputMask, std::move(data_mask));
  }
  
  using TxtFileReaderToDataObjectCollectionAlgorithm = TxtFileReaderAlgorithm<EventDataModel::DataObjectCollection>;
  using TXTFileReaderToMultiDataObjectCollectionAlgorithm = TxtFileReaderAlgorithm<EventDataModel::MultiDataObjectCollection>;
}

#endif

