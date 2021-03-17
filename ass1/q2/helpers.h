int getCommentEnd(char *, int);
void processComments(char *, int, FILE *, int *);
int getNonSpace(char *, int);
void processRedunSpaces(char *, int, FILE *, int *);
void cleanupLine(char *, FILE *, int *);
int getQuotesEnd(char *, int);
void cleanupLeftoverLine(char *, FILE *, int *);