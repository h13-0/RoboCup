#pragma once

extern "C"
{
    void* Init(char *TargetFrameBufferFile);
    void Display(void* FrameBufferHandle, unsigned char* Image, int Rows, int Cols);
    int GetFrameBufferX_Resolution(void* FrameBufferHandle);
    int GetFrameBufferY_Resolution(void* FrameBufferHandle);
    void DeInit(void* FrameBufferHandle);
}
