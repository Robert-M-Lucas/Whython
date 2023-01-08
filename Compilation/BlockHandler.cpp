//
// Created by rober on 23/12/2022.
//

#include "BlockHandler.h"
#include "../Blocks/LoopBlock.h"
#include "../Blocks/WhileBlock.h"
#include "../Blocks/IfBlock.h"
#include "LineHandler.h"

extern bool DEBUG;

BlockHandler::BlockHandler(BlockHandler *above, BlockController *controller, MemoryManager *pManager,
                           MemoryManager *mManager) {
    Above = above;
    if (Above == nullptr)
        Depth = 0;
    else
        Depth = (Above->Depth) + 1;

    Controller = controller;
    PManager = pManager;
    VManager = mManager;
    References = new ReferenceHandler();
}

Reference *BlockHandler::RecursivelyGetReferenceOrNull(const string &referenceName) {
    BlockHandler* handler = this;
    while (handler != nullptr) {
        Reference* reference = handler->References->GetReferenceOrNull(referenceName);
        if (reference != nullptr) return reference;
        handler = handler->Above;
    }
    return nullptr;
}

Reference *BlockHandler::RecursivelyGetReference(const string &referenceName) {
    Reference* reference = BlockHandler::RecursivelyGetReferenceOrNull(referenceName);
    if (reference == nullptr)
        throw invalid_argument("Name '" + referenceName + "' not found");
    return reference;
}

BlockHandler *BlockHandler::OnLineParsed(vector<LexicalResult> parsedLine) {
    // Depth went up without new block
    if (parsedLine[0].Depth > Depth) { throw invalid_argument("Depth formatting exception"); }

    // Go down depth (exit current block)
    if (parsedLine[0].Depth < Depth) {
        if (DEBUG)
            cout << "Down depth" << F_ENDL;
        if (OnExit(parsedLine, Depth - parsedLine[0].Depth)) {
            if (DEBUG)
                cout << "Down depth succeeded" << F_ENDL;
            BlockHandler* ret = Above->OnLineParsed(parsedLine);
            delete this;
            return ret;
        }
        else {
            return this;
        }
    }

    // Handle block
    if (parsedLine[0].Type == Blocker) {
        if (DEBUG)
            cout << "Up depth" << F_ENDL;
        string blocker = parsedLine[0].Value;
        BlockController* new_controller;
        /*
        if (blocker == "loop") {
            new_controller = new LoopBlock();
        }
        else */
        if (blocker == "while") {
            new_controller = new WhileBlock();
        }
        else if (blocker == "if") {
            new_controller = new IfBlock();
        }
        else {
            throw runtime_error("Unsupported block '" + blocker + "'");
        }
        auto* newBlockHandler = new BlockHandler(this, new_controller, PManager, VManager);
        new_controller->OnEnter(this, newBlockHandler, parsedLine);
        return newBlockHandler;
    }
    // Normal handle
    else {
        HandleLine(parsedLine, this);
    }

    return this;
}

BlockHandler::~BlockHandler() {
    delete References;
    delete Controller;
}

bool BlockHandler::OnExit(const vector<LexicalResult>& exitingLine, int depthDelta) {
    if (Controller == nullptr)
        cout << "Null controller" << F_ENDL;
    return Controller->OnExit(this, exitingLine, depthDelta);
}
