#include "global.h"
#include "kryocore.h"

bool ChunkTest()
{
    // Grandeur du chunk
    Kryo::Chunk* testChunk = new Kryo::Chunk();
    int chunkSize = testChunk->GetBlockCount();
    if (chunkSize != 32768)
        return false;

    // Get et Set d'un block
    testChunk->Set(0, 0, 0, BTYPE_GRASS);
    if (testChunk->Get(0, 0, 0) != BTYPE_GRASS)
        return false;

    testChunk->Set(15, 100, 12, BTYPE_DIRT);
    if (testChunk->Get(15, 100, 12) != BTYPE_DIRT)
        return false;

    return true;
}

bool BlockTest()
{
    Kryo::BlocInfo* testBlock = new Kryo::BlocInfo(BTYPE_GRASS, "grass");
    testBlock->SetDurabilite(5);
    testBlock->Afficher();
    if (testBlock->GetDurabilite() != 5)
        return false;

    return true;
}

int main(int argc, char** argv)
{
    std::cout << "Chunk test: " << (ChunkTest() ? "passed" : "failed") << std::endl;
    std::cout << "Block test: " << (BlockTest() ? "passed" : "failed") << std::endl;
}
