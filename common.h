#pragma once

// skip 'restrict' C keyword in C++
#ifdef __cplusplus
# define restrict __restrict__
#endif
