#define INITIAL_TABLE_SIZE 100
#define LOAD_FACTOR 0.75

typedef struct KeyValue KeyValue;
typedef struct HashMap HashMap;

typedef struct KeyValue {
  char* key;
  void* value;
  struct KeyValue* next;
} KeyValue;

typedef struct HashMap {
  KeyValue** hashTable;
  unsigned int size;
  unsigned int load;
} HashMap;

HashMap* createHashMap(int size);
KeyValue* findKeyValue(HashMap* hashMap, const char* id);
void resizeHashMap(HashMap** hashMap);
int addValue(HashMap** hashMap, void* value, const char* key);
int removeValue(HashMap* hashMap, const char* key);
int hashFunction(const char* hashString, int hashMapSize);
void printHashMap(HashMap* hashMap);
void destroyHashMap(HashMap* hashMap);