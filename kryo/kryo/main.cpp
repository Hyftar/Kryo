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
    if (testChunk->GetBloc(0, 0, 0) != BTYPE_GRASS)
        return false;

    testChunk->SetBloc(15, 100, 12, BTYPE_DIRT);
    if (testChunk->GetBloc(15, 100, 12) != BTYPE_DIRT)
        return false;

    // Test de copie d'un chunk
    Kryo::Chunk* testCloneChunk = new Kryo::Chunk(*testChunk);
    if (testCloneChunk->GetBloc(15, 100, 12) != BTYPE_DIRT)
        return false;

    testCloneChunk->RemoveBloc(15, 100, 12);
    if (testChunk->GetBloc(15, 100, 12) == BTYPE_AIR || testCloneChunk->GetBloc(15, 100, 12) != BTYPE_AIR)
        return false;

    testCloneChunk->Reset(BTYPE_GRASS);
    if (testCloneChunk->GetBloc(8, 111, 12) != BTYPE_GRASS || testChunk->GetBloc(8, 111, 12) != BTYPE_AIR)
        return false;

    testChunk->Set(10, 11, 5, BTYPE_GRASS);
    if (testChunk->Get(2906) != BTYPE_GRASS)
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
