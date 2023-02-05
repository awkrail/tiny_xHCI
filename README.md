# tiny_xHCI
This repo is an implementation for xHCI host device driver written in C.

# Memo
- memcpy -> memory is not defined, so this cannot be used for this purpose
- (struct CapablityRegister*)cap -> strict aliasing rule violation -> -O2 doesn't work
- Solution: volatile is only the way to avoid this bug in -O2 
