#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define JSON_true 0
#define JSON_false 1
#define JSON_null 2
#define JSON_number 3
#define JSON_string 4
#define JSON_array 5
#define JSON_object 6

static const char* expression;
typedef struct json {
	struct json* prev,*next;
	struct json* child;
		int type;
		char* valueString;
		char* nameString;
		int valueInt;
		double ValueDouble;
} JSON;

static const char* parse_value(JSON* node, const char* value);
static const char* parse_array(JSON* item, const char* str);

static JSON* c_json_newItem() {
	JSON* node = (JSON*)malloc(sizeof(JSON));
	if (node)  memset(node, 0, sizeof(JSON));
	else printf("内存error");
	return node;
}

static const char * skip(const char* in) {
	while (in && *in && (unsigned char)*in <= 32)
		in++;
	return in;
}
static const char * parse_number(JSON* item,const char* str) {
	double n = 0,sign = 1, scale = 0;
	if (*str == '-') sign = -1,str++;
	if (*str == '0') str++;
	if (*str >= '0' && *str <= '9') {
		do
		{
			n = 10.0 * n + (*str++ - '0');
		} while (*str >= '0' && *str <= '9');
	}
	if (*str == '.' && str[1] >= '0' && str[1] <= '9') {
		str++;
		do
		{
			n = 10 * n + (*str++ - '0');
			scale--;
		} while (*str >= '0' && *str <= 9);
	}
	n = sign * n * pow(10.0, scale);
	item->type = JSON_number;
	item->ValueDouble = n;
	item->valueInt = (int)n;
	return str;
}

static const char* parse_string(JSON * item, const char* str) { 
	const char* pos = str + 1;
	char* out;
	char* str2;
	int len = 0;
	if (*str != '\"') {
		expression = str;
		return 0;
	}
	while (*pos != '\"' && *pos && ++len) if (*pos++ == '\\') pos++;
	out = malloc(len + 1);
	if (!out) return 0;
	pos = str + 1, str2 = out;
	while (*pos != '\"')
	{
		if (*pos != '\\') *str2++ = *pos++;
		else {
			pos++;
			*str2++ = *pos++;
		}
	}
	*str2 = '\0';
	if (*pos == '\"') pos++;
	item->type = JSON_string;
	item->valueString = out;
	return pos;
}
static const char* parse_array(JSON *item, const char *str){ 
	JSON* child;
	if (*str != '[') { 
		expression = str;
		return 0;
	}
	item->type = JSON_array;
	str = skip(str + 1);
	if (*str == ']') return str + 1;
	item->child = child = c_json_newItem();
	str = skip(parse_value(child, str));
	if (!str) return 0;
	while (*str == ',')
	{
		JSON* nextChild = c_json_newItem();
		child->next = nextChild, nextChild->prev = child, child = nextChild;
		str = skip(parse_value(child, skip(str + 1)));
		if (!str) return 0;
	}
	if (*str == ']') return str + 1;
	expression = str;
	return 0;
}
static const char* parse_object(JSON*item, const char *str) {
	JSON* child;
	if (*str != '{') {
		expression = str;
		return 0;
	}
	item->type = JSON_object;
	str = skip(str + 1);
	if (*str == '}') return str + 1;
	item->child = child = c_json_newItem();
	if (!item->child) return 0;

	str = skip(parse_string(child, str));
	if (!str) return 0;
	child->nameString = child->valueString;
	child->valueString = 0;
	if (*str != ':') { 
		expression = str;  
		return 0;
	}
	str =  skip(parse_value(child, skip(str + 1)));
	if (!str) return 0;
	while (*str == ',')
	{
		JSON* nextChild = c_json_newItem();
		child->next = nextChild, nextChild->prev = child, child = nextChild;
		str = skip(parse_string(child, skip(str + 1)));
		if (!str) return 0;
		child->nameString = child->valueString;
		child->valueString = 0;
		if (*str != ':') {
			expression = str;
			return 0;
		}
		str = skip(parse_value(child, skip(str + 1)));
		if (!str) return 0;
	}
	if (*str == '}') return str + 1;
	expression = str;
	return 0;
}

static const char * parse_value(JSON* node,const char*value) {
	if (!value) return 0;
	if (!strncmp(value, "null", 4)) {
		node->type = JSON_null;
		node->valueString = "null";
		return value + 4;
	}
	if (!strncmp(value, "true", 4)) {
		node->type = JSON_true;
		node->valueString = "true";
		return value + 4;
	}
	if (!strncmp(value, "false", 5)) {
		node->type = JSON_false;
		node->valueString = "false";
		return value + 5;
	}																				 
	if (*value == '-' || (*value >= '0' && *value <= '9')) {
		return parse_number(node, value);
	}
	if (*value == '\"') { return parse_string(node, value); }
	if (*value == '{') {
		return parse_object(node, value);
	}
	if (*value == '[') {
		return parse_array(node, value);
	}
	expression = value;
	return 0;
}

JSON* parse_options(const char * text,const char **require_end)
{
	 const char *end = 0;
	JSON* c;
	c = c_json_newItem();
	expression = "初始化json节点执行: c_json_newItem";
	if (!c) return 0;
	end = parse_value(c, skip(text));
	if (require_end) *require_end = end;
	return c;
}

JSON* parse(const char* text) { return parse_options(text, 0); }

char* json_print(JSON* json) {
	char* p = "待完成...";
	return p;
}

static const char* ErrorCatch() {
	return expression;
}
void doInit(const char*text) {
	char* out; JSON* json;
	json = parse(text);
	if (!json)  printf("Error position: %s\n", ErrorCatch());
	out = json_print(json);
}


void writeInFile() {

}

void generateInit(const char *arr[],int size) {
	for (int i = 0; i < size; i++)
	{
		doInit(arr[i]);
		/*for (int j = 0; j < 30; j++) {
			printf("arr[%d][%d]=%c\n", i,j, arr[i][j]);
		}*/
		//printf("arr[%d]=%s\n",i,arr[i]);
	}
}

int main(int argc, const char * argv) {
	char text1[] = "{\n\"name\": \"Jack (\\\"Bee\\\") Nimble\", \n\"format\": {\"type\":       \"rect\", \n\"width\":      1920, \n\"height\":     1080, \n\"interlace\":  false,\"frame rate\": 24\n}\n}";
	char text2[] = "[\"Sunday\", \"Monday\", \"Tuesday\", \"Wednesday\", \"Thursday\", \"Friday\", \"Saturday\"]";
	char text3[] = "[\n    [0, -1, 0],\n    [1, 0, 0],\n    [0, 0, 1]\n	]\n";
	char text4[] = "{\n		\"Image\": {\n			\"Width\":  800,\n			\"Height\": 600,\n			\"Title\":  \"View from 15th Floor\",\n			\"Thumbnail\": {\n				\"Url\":    \"http:/*www.example.com/image/481989943\",\n				\"Height\": 125,\n				\"Width\":  \"100\"\n			},\n			\"IDs\": [116, 943, 234, 38793]\n		}\n	}";
	char text5[] = "[\n	 {\n	 \"precision\": \"zip\",\n	 \"Latitude\":  37.7668,\n	 \"Longitude\": -122.3959,\n	 \"Address\":   \"\",\n	 \"City\":      \"SAN FRANCISCO\",\n	 \"State\":     \"CA\",\n	 \"Zip\":       \"94107\",\n	 \"Country\":   \"US\"\n	 },\n	 {\n	 \"precision\": \"zip\",\n	 \"Latitude\":  37.371991,\n	 \"Longitude\": -122.026020,\n	 \"Address\":   \"\",\n	 \"City\":      \"SUNNYVALE\",\n	 \"State\":     \"CA\",\n	 \"Zip\":       \"94085\",\n	 \"Country\":   \"US\"\n	 }\n	 ]";
	char text6[] = "  null";
	char text7[] = "true";
	char text8[] = "false ";
	char text9[] = "56 ";
	char text10[] = "-56";
	char *allText[5] = { text1,text2, text3,text4,text5 };
	char* allText2[5] = { text6,text7,text8,text9,text10 };
	char* temp[1] = { text3 };
	//generateInit(temp, 1);
	//generateInit(allText2, 5);
	generateInit(allText, 5);
}