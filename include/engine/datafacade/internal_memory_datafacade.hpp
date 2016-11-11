#ifndef INTERNAL_DATAFACADE_HPP
#define INTERNAL_DATAFACADE_HPP

// implements all data storage when shared memory is _NOT_ used

#include "engine/datafacade/bigramblock_datafacade_base.hpp"
#include "storage/storage.hpp"

namespace osrm
{
namespace engine
{
namespace datafacade
{

/**
 * This datafacade uses a process-local memory block to load
 * data into.  The structure and layout is the same as when using
 * shared memory, so this class, and the SharedDatafacade both
 * share a common base.
 * This class holds a unique_ptr to the memory block, so it
 * is auto-freed upon destruction.
 */
class InternalDataFacade final : public BigRAMBlockDataFacadeBase
{

  private:
    std::unique_ptr<char[]> internal_memory;
    std::unique_ptr<storage::DataLayout> internal_layout;

  public:
    explicit InternalDataFacade(const storage::StorageConfig &config)
    {
        storage::Storage storage(config);

        // Calculate the layout/size of the memory block
        internal_layout = std::make_unique<storage::DataLayout>();
        storage.LoadLayout(internal_layout.get());

        // Allocate the memory block, then load data from files into it
        internal_memory.reset(new char[internal_layout->GetSizeOfLayout()]);
        storage.LoadData(internal_layout.get(), internal_memory.get());

        // Adjust all the private m_* members to point to the right places
        Init(internal_layout.get(), internal_memory.get());
    }
};
}
}
}

#endif // INTERNAL_DATAFACADE_HPP