#ifndef SUSARGPARSE_H
#define SUSARGPARSE_H

extern int susargparse_afterparse;

int susargparse_option(int argc, char **argv, char *cmd);
int susargparse_value(int argc, char **argv, char *cmd);

#endif
