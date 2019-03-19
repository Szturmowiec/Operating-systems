struct bloki
{
	char **t;
	int ile;
};

struct bloki* create_dynamic(int n,int size);
void delete_dynamic(struct bloki* t);
struct bloki* create_static();
void add_dynamic(struct bloki* t,char* s,int size);
void add_static(struct bloki* t,char* s);
void remove_dynamic(struct bloki* t,int id);
void remove_static(struct bloki* t,int id);
char* nearest_value(struct bloki* t,int n,int size,int x);