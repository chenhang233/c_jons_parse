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
		JSON* prev,*next;
		JSON* child;
		int type;
		char* valueString;
		char* nameString;
		int valueInt;
		double ValueDouble;
} JSON;

static JSON* c_json_newItem() {
	JSON* node = (JSON*)malloc(sizeof(JSON));
	if (node)  memset(node,0, sizeof(JSON);
	return node;
}

static const skip(char* in) {
	while (in && *in && (unsigned char)*in <= 32)
		in++;
	return in;
}
static const parse_number(JSON* item, char* str) {
	double n,sign = 1, scale = 0;
	if (*str == '-') sign = -1;
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
static const parse_string(JSON*item, char *str) {}
static const parse_array(JSON *item, char *str){}
static const parse_object(JSON*item, char *str) {}

char * parse_value(JSON* node, char*value) {
	if (!value) return 0;
	if (!strncmp(value, "null", 4)) {
		node->type = JSON_null;
		return value + 4;
	}
	if (!strncmp(value, "true", 4)) {
		node->type = JSON_true;
		return value + 4;
	}
	if (!strncmp(value, "false", 5)) {
		node->type = JSON_false;
		return value + 5;
	}
	if (*value == '-' || (*value <= 0 && *value >= 9)) {
		node->type = JSON_number;
		return parse_number(node, value);
	}
	if (*value == '\"') { return parse_string(node, value); }
	if (*value == '{') {
		node->type = JSON_object;
		return parse_object(node, value);
	}
	if (*value == '[') {
		node->type = JSON_array;
		return parse_array(node, value);
	}
}

void parse_options(char * text, char **require_end)
{
	const char *end;
	JSON* c;
	c = c_json_newItem();
	expression = "初始化node执行: c_json_newItem";
	if (!c) return 0;
	end = parse_value(c, skip(text));
	if (require_end) *require_end = end;
	return c;
}

JSON* parse(char* text) { return parse_options(text, 0); }

char* json_print(JSON* json) {

}

void ErrorCatch() {
	return expression;
}
void doInit(char*text) {
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
		printf("arr[%d]=%s",i,arr[i]);
	}
}

int main(int argc, const char * argv) {
	char text1[] = "{\n\"name\": \"Jack (\\\"Bee\\\") Nimble\", \n\"format\": {\"type\":       \"rect\", \n\"width\":      1920, \n\"height\":     1080, \n\"interlace\":  false,\"frame rate\": 24\n}\n}";
	char text2[] = "[\"Sunday\", \"Monday\", \"Tuesday\", \"Wednesday\", \"Thursday\", \"Friday\", \"Saturday\"]";
	char text3[] = "[\n    [0, -1, 0],\n    [1, 0, 0],\n    [0, 0, 1]\n	]\n";
	char text4[] = "{\n		\"Image\": {\n			\"Width\":  800,\n			\"Height\": 600,\n			\"Title\":  \"View from 15th Floor\",\n			\"Thumbnail\": {\n				\"Url\":    \"http:/*www.example.com/image/481989943\",\n				\"Height\": 125,\n				\"Width\":  \"100\"\n			},\n			\"IDs\": [116, 943, 234, 38793]\n		}\n	}";
	char text5[] = "[\n	 {\n	 \"precision\": \"zip\",\n	 \"Latitude\":  37.7668,\n	 \"Longitude\": -122.3959,\n	 \"Address\":   \"\",\n	 \"City\":      \"SAN FRANCISCO\",\n	 \"State\":     \"CA\",\n	 \"Zip\":       \"94107\",\n	 \"Country\":   \"US\"\n	 },\n	 {\n	 \"precision\": \"zip\",\n	 \"Latitude\":  37.371991,\n	 \"Longitude\": -122.026020,\n	 \"Address\":   \"\",\n	 \"City\":      \"SUNNYVALE\",\n	 \"State\":     \"CA\",\n	 \"Zip\":       \"94085\",\n	 \"Country\":   \"US\"\n	 }\n	 ]";
	//doInit(text1);
	char *allText[5] = { text1,text2, text3,text4,text5 };
	generateInit(allText, 5);
}