#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Analysis/LoopPass.h"

using namespace llvm;

namespace {
   struct MyPass : public LoopPass {
      static char ID;
      MyPass() : LoopPass(ID) {}

      bool runOnLoop(Loop *L, LPPassManager &LPM) {
         L->dump();
         for (unsigned i = 0; i != L->getBlocks().size(); i++) {
            auto& BB = L->getBlocks()[i];

            for(auto& Instr: *BB) {
               auto instr = dyn_cast<GetElementPtrInst>(&Instr);

               if (instr == nullptr)
                  continue;

               if(auto resPtrType = dyn_cast<PointerType>(instr->getResultElementType())) {
                  if (auto resType = dyn_cast<StructType>(resPtrType->getPointerElementType())) {
                     for (auto&E : resType->elements()) {
                        if (resPtrType == E) {
                           errs() << "Instr: ";
                           instr->dump();

                           errs() << "Potential Pointer Prefetch\n";
                           break;
                        }
                     }
                  }
               }
            }
         }
         errs() << "\n";
         return false;
      }
   };
}

char MyPass::ID = 0;
static RegisterPass<MyPass> X("mypass","My First Pass",true,false);
