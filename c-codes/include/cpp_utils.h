#ifndef _CPP_UTILS_H_
#define _CPP_UTILS_H_
/******************************************************************************/
/*
 * Link std::cout to a file.
 */
void link_cout(const char *fname);

/*
 * Unlink and restore std::cout redirected by link_cout.
 */
void unlink_cout();
/******************************************************************************/
#endif
