import CPPModule from './mancalaBoard.js'
var wasmInstance = await CPPModule();
//这个题的关键就是看看怎么返回一个js数组
function mancalaBoard(flag, seq, size) {
    const cppFuncPtr = wasmInstance.cwrap('mancala_board', 'number', ['number', 'number', 'number']);
    const malloc = wasmInstance.cwrap('malloc', 'number', ['number']);
    const free = wasmInstance.cwrap('free',null,['number']);
    const paramPtr= malloc(seq.length*Uint32Array.BYTES_PER_ELEMENT)
    const memoryView = new Uint32Array(wasmInstance.HEAPU8.buffer,paramPtr,size)
    for(let i=0;i<size;i++){
        memoryView[i]=seq[i];
    }
    const ansPtr= cppFuncPtr(flag,paramPtr,size)
    //fixme ansMemoryView的size注意了
    const ansMemoryView = new Uint32Array(wasmInstance.HEAPU8.buffer,ansPtr,20)
    const ansArray=[]
    for(let i=0;i<15;i++){
        ansArray.push(ansMemoryView[i])
    }
    free(paramPtr);
    return ansArray
}
export {mancalaBoard}