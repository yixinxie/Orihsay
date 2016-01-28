#pragma once
#define SAFE_RELEASE(h) if(h != nullptr){(h)->Release(); (h) = nullptr;}
#define SAFE_DISPOSE(h) if((h) != nullptr){(h)->dispose(); delete (h); (h) = nullptr;}