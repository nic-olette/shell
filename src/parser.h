#ifndef PARSER_H_   
#define PARSER_H_

struct Flags {
  int pipeflag;
 // int dollarflag;
  int redirINflag;
  int redirOUTflag;
  int cdflag;
};

void parse(char *line, char **args, struct Flags *fl);

#endif // PARSER_H_
