#ifndef HotHand_Data
#define HotHand_Data

#include <vector>

namespace HotHand
{

class Data
{
    private:
        // shots/failure for a bunch of shots
        std::vector<bool> shots;

    public:
        Data();

        // Load from text file
        void load(const char* filename);

        // Getter
        const std::vector<bool>& get_shots() const
        { return shots; }

        // Global instance
        static Data instance;
};

} // namespace HotHand

#endif

