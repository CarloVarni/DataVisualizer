
#ifndef ROOT_FILE_READER_H
#define ROOT_FILE_READER_H 1

#include <BaseAlgorithm.hpp>
#include <TFile.h>

namespace Algorithm {

  class RootFileReaderAlgorithm
    : public Core::BaseAlgorithm {

  public:
    struct Config {
      std::string fileName;
      std::string treeName;
      std::string dataCollectionName;
    };

  public:
    RootFileReaderAlgorithm(const std::string& name,
			    const Config& cfg);
    RootFileReaderAlgorithm(const RootFileReaderAlgorithm&) = delete;
    RootFileReaderAlgorithm& operator=(const RootFileReaderAlgorithm&) = delete;
    virtual ~RootFileReaderAlgorithm() override = default;

    virtual void initialize() override;
    virtual void execute(Core::EventContext& context) override;
    virtual void finalize() override;
    
  private:
    const Config m_cfg;
    std::unique_ptr<TFile> m_file;
  };
  
}


#endif
