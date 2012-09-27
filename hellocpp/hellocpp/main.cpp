#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

/*
 *****************************
 * CODE SET (from .tbl file) *
 *****************************
 */
 
struct code
{
	unsigned char *str;
	int len;
	unsigned int value;
	struct code *next;
};

struct code_set
{
	struct code *begin;
	struct code *end;
	unsigned int size;
};

int code_set_append(struct code_set *set, unsigned char *str, int len, unsigned int value)
{
	struct code *new_code = (struct code *)malloc(sizeof(struct code));
	if (new_code == NULL) return -1;
	new_code->next = NULL;
	new_code->str = str;
	new_code->len = len;
	new_code->value = value;
	if (set->begin == NULL)
	{
		set->begin = set->end = new_code;
	}
	else
	{
		set->end->next = new_code;
		set->end = new_code;
	}
	set->size++;
	return 0;
}

int code_set_walk(struct code_set *set)
{
	struct code *cur_code = set->begin, *next_code;
	while (cur_code)
	{
		next_code = cur_code->next;
		printf("%04X->", cur_code->value);
		fwrite(cur_code->str, 1, cur_code->len, stdout);
		printf("\n");
		cur_code = next_code;
	}
	return 0;
}

struct code *code_set_find(struct code_set *set, unsigned int value)
{
	struct code *cur_code = set->begin;
	while (cur_code)
	{
		if (cur_code->value == value) return cur_code;
		cur_code = cur_code->next;
	}
	return NULL;
}

int streq_n(unsigned char* a, const char* b, int len){
	while(len){
		--len;
		if(((char)a[len])!=b[len])
			return 0;
	}
	return 1;
}

struct code* code_set_find_value(struct code_set *set, char* str)
{
	struct code *cur_code = set->begin;
	while (cur_code)
	{
		if (cur_code->len!=0 && streq_n(cur_code->str, str, cur_code->len))
			return cur_code;
		cur_code = cur_code->next;
	}
	return NULL;
}

int code_set_destroy(struct code_set *set)
{
	struct code *cur_code = set->begin, *next_code;
	while (cur_code)
	{
		next_code = cur_code->next;
		free(cur_code);
		cur_code = next_code;
	}
	free(set);
	return 0;
}

#define PARSE_CODE_STATE_CODE 0
#define PARSE_CODE_STATE_TEXT 1

unsigned int codehexptou32(unsigned char *p, int len)
{
	unsigned int value = 0;
	unsigned int digit;

	while (len)
	{
		len--;
		if (('0' <= *p) && (*p <= '9'))
		{
			digit = *p - '0';
		}
		else if (('A' <= *p) && (*p <= 'F'))
		{
			digit = (*p - 'A') + 10;
		}else continue;
		value = (value << 4) | digit;
		p++;
	}
	return value;
}

struct code_set *parse_code_set(unsigned char *data, unsigned int data_len)
{
	int ret;

	int state = PARSE_CODE_STATE_CODE;

	unsigned char *new_code_str;
	unsigned int new_code_value;
	unsigned char *new_code_p;

	struct code_set *set = (struct code_set *)malloc(sizeof(struct code_set));
	if (set == NULL) return NULL;
	set->begin = set->end = NULL;
	set->size = 0;
	unsigned char *data_p = data, *data_endp = data + data_len;
	if (data_len < 3) return NULL;
	/* utf-8 check */
	if (data[0] == 0xEF && data[1] == 0xBB && data[2] == 0xBF)
	{
		data_p += 3;
	}
	/* start parsing */
	new_code_p = data_p;
	while (data_p != data_endp)
	{
		if (*data_p == 0x0D && *(data_p + 1) == 0x0A)
		{
			ret = code_set_append(set, new_code_str, data_p - new_code_str, new_code_value);
			if (ret) return NULL;
			data_p += 2;
			new_code_p = data_p;
			state = PARSE_CODE_STATE_CODE;
		}
		else if (*data_p == '=')
		{
			/* get a code */
			new_code_value = codehexptou32(new_code_p, data_p - new_code_p);
			data_p++;
			new_code_str = data_p;
			state = PARSE_CODE_STATE_TEXT;
		}
		else
		{
			data_p++;
		}
	}
	/* parse the final one code */
	if (data_p - new_code_str)
	{
		ret = code_set_append(set, new_code_str, data_p - new_code_str, new_code_value);
		if (ret) return NULL;
	}

	return set;
}

/*
 *************************************
 * TEXT SECTION SET (from .bin file) *
 *************************************
 */

struct section
{
	unsigned int offset;
	struct section *next;
};

struct section_set
{
	struct section *begin;
	struct section *end;
	unsigned int size;
};

int section_set_append(struct section_set *set, unsigned int offset)
{
	struct section *new_section = (struct section *)malloc(sizeof(struct section));
	if (new_section == NULL) return -1;
	new_section->next = NULL;
	new_section->offset = offset;
	if (set->begin == NULL)
	{
		set->begin = set->end = new_section;
	}
	else
	{
		set->end->next = new_section;
		set->end = new_section;
	}
	set->size++;
	return 0;
}

int section_set_walk(struct section_set *set)
{
	struct section *cur_section = set->begin, *next_section;
	while (cur_section)
	{
		next_section = cur_section->next;
		printf("offset=%u\n", cur_section->offset);
		cur_section = next_section;
	}
	return 0;
}

int section_set_destroy(struct section_set *set)
{
	struct section *cur_section = set->begin, *next_section;
	while (cur_section)
	{
		next_section = cur_section->next;
		free(cur_section);
		cur_section = next_section;
	}
	free(set);
	return 0;
}

struct section_set *parse_section_set(unsigned char *data, unsigned int data_len)
{
	struct section_set *set = (struct section_set *)malloc(sizeof(struct section_set));
	if (set == NULL) return NULL;
	set->begin = set->end = NULL;
	set->size = 0;
	unsigned char *data_p = data;
	/* read section size */
	unsigned int section_count;
	section_count = *((unsigned int *)data_p); data_p += 4;
	int i;
	unsigned int offset;
	for (i = 0; i < section_count; i++)
	{
		offset = *((unsigned int *)data_p); data_p += 4;
		section_set_append(set, offset);
	}
	return set;
}

/*
 ****************
 * TEXT REPLACE *
 ****************
 */

#define KANJI_PLUS_KANA_STATE_INSIDE 0
#define KANJI_PLUS_KANA_STATE_OUTSIDE 1

#define SENTENCE_BUFFER_SIZE (4096*8)

int text_replace(FILE *fp_txt, unsigned char *data_bin, unsigned int data_bin_len,
		struct section_set *section_set,
		struct code_set *code_set)
{
	int i;
	int section_idx = 0;
	int kanji_plus_kana_state;
	unsigned char *data_bin_p, *data_bin_endp;
	struct section *cur_section = section_set->begin;
	unsigned int length;
	struct code *matched_code;
	unsigned char sentence_buffer[SENTENCE_BUFFER_SIZE];
	unsigned char *sentence_buffer_p;
	unsigned int write_size;

	unsigned char *data_bin_p_mark;

	while (cur_section)
	{
		section_idx++;
		/* SECTION ID */
		fprintf(fp_txt, "No.%d\n", section_idx);
		/* 1st seperator */
		fprintf(fp_txt, "######\n");
		kanji_plus_kana_state = KANJI_PLUS_KANA_STATE_OUTSIDE;
		data_bin_p = data_bin + cur_section->offset;
		if (cur_section->next != NULL)
			length = cur_section->next->offset - cur_section->offset;
		else
			length = data_bin_len - cur_section->offset;
		data_bin_endp = data_bin_p + length;

		memset(sentence_buffer, 0, SENTENCE_BUFFER_SIZE);
		sentence_buffer_p = sentence_buffer;

		while (data_bin_endp - data_bin_p > 0)
		{
			if (*data_bin_p == 0x1B && *(data_bin_p + 1) == 0x63)
			{
				/* special control code, 0x1B, 0x63, <parameter> */
				write_size = sprintf((char *)sentence_buffer_p, "<%02X%02X%02X>", *data_bin_p, *(data_bin_p + 1), *(data_bin_p + 2));
				sentence_buffer_p += write_size;
				data_bin_p += 3;
			}
			else if ((*data_bin_p == 0x25 && *(data_bin_p + 1) == 0x73) ||
				(*data_bin_p == 0x25 && *(data_bin_p + 1) == 0x79))
			{
				write_size = sprintf((char *)sentence_buffer_p, "<%02X%02X>", *data_bin_p, *(data_bin_p + 1));
				sentence_buffer_p += write_size;
				data_bin_p += 2;
			}
			else if (*data_bin_p == 0x1B && *(data_bin_p + 1) == 0x73)
			{
				write_size = sprintf((char *)sentence_buffer_p, "<%02X%02X%02X>", *data_bin_p, *(data_bin_p + 1), *(data_bin_p + 2));
				sentence_buffer_p += write_size;
				data_bin_p += 3;
			}
			else if (*data_bin_p == 0x1B && *(data_bin_p + 1) == 0x66)
			{
				write_size = sprintf((char *)sentence_buffer_p, "<%02X%02X%02X>", *data_bin_p, *(data_bin_p + 1), *(data_bin_p + 2));
				sentence_buffer_p += write_size;
				data_bin_p += 3;
			}
			else if (*data_bin_p == 0x05 && *(data_bin_p + 1) == 0x05 && *(data_bin_p + 2) == 0x42 && *(data_bin_p + 3) == 02)
			{
				write_size = sprintf((char *)sentence_buffer_p, "<%02X%02X%02X%02X>", *data_bin_p, *(data_bin_p + 1), *(data_bin_p + 2), *(data_bin_p + 3));
				sentence_buffer_p += write_size;
				data_bin_p += 4;
			}
			else if (*data_bin_p == 0x25)
			{
				/* 0x25, %0..%129, 0x01, 0x22*/
				data_bin_p_mark = data_bin_p;
				while (!(*data_bin_p == 0x01 && *(data_bin_p + 1) == 0x22))
				{
					data_bin_p++;
				}
				unsigned int percent_code_len = data_bin_p - data_bin_p_mark;

				*sentence_buffer_p++ = '<';
				for (i = 0; i < percent_code_len; i++)
				{
					write_size = sprintf((char *)sentence_buffer_p, "%02X", *(data_bin_p_mark + i));
					sentence_buffer_p += write_size;
				}
				*sentence_buffer_p++ = '>';
			}
			else if (*data_bin_p == 0x02)
			{
				write_size = sprintf((char *)sentence_buffer_p, "<%02X%02X%02X>", *data_bin_p, *(data_bin_p + 1), *(data_bin_p + 2));
				sentence_buffer_p += write_size;
				data_bin_p += 3;
			}
			else if (*data_bin_p == 0x1B && *(data_bin_p + 1) == 0x72)
			{
				/* kana + kanji */
				if (kanji_plus_kana_state == KANJI_PLUS_KANA_STATE_OUTSIDE)
				{
					kanji_plus_kana_state = KANJI_PLUS_KANA_STATE_INSIDE;
				}
				else
				{
					kanji_plus_kana_state = KANJI_PLUS_KANA_STATE_OUTSIDE;
				}
				
				if (kanji_plus_kana_state == KANJI_PLUS_KANA_STATE_INSIDE)
				{
					*sentence_buffer_p = '{';
					sentence_buffer_p++;
				}
				if (kanji_plus_kana_state == KANJI_PLUS_KANA_STATE_OUTSIDE)
				{
					*sentence_buffer_p = '}';
					sentence_buffer_p++;
				}
				data_bin_p += 2;
			}
			else if (*data_bin_p == 0x2F)
			{
				write_size = sprintf((char *)sentence_buffer_p, "|");
				sentence_buffer_p += write_size;
				data_bin_p++;
			}
			else if (*data_bin_p == 0x0A &&
					*(data_bin_p + 1) == 0x01 &&
					*(data_bin_p + 2) == 0x22)
			{
				/* eol */
				write_size = sprintf((char *)sentence_buffer_p, "<EOL>\n");
				sentence_buffer_p += write_size;
				data_bin_p += 3;
			}
			else if (*data_bin_p == 0x05 &&
					*(data_bin_p + 1) == 0x05 &&
					*(data_bin_p + 2) == 0x05)
			{
				/* end */
				write_size = sprintf((char *)sentence_buffer_p, "<%02X%02X%02X>\n", *data_bin_p, *(data_bin_p + 1), *(data_bin_p + 2));
				sentence_buffer_p += write_size;
				data_bin_p += 3;
			}
			else
			{
				unsigned int value;
				unsigned int value_len;
				if (*data_bin_p < 0x40)
				{
					value = *data_bin_p;
					value_len = 1;
				}
				else
				{
					value = (*(data_bin_p + 1)) | ((*data_bin_p) << 8);
					value_len = 2;
				}
				matched_code = code_set_find(code_set, value);
				/*matched_code = code_set_find(code_set, *(data_bin_p + 1) | (*(data_bin_p) << 8));*/
				if (matched_code)
				{
					memcpy(sentence_buffer_p, matched_code->str, matched_code->len);
					sentence_buffer_p += matched_code->len;
				}
				else
				{
					if (value_len == 1)
					{
						write_size = sprintf((char *)sentence_buffer_p, "<%02X>", *data_bin_p);
					}
					else
					{
						write_size = sprintf((char *)sentence_buffer_p, "<%02X%02X>", *data_bin_p, *(data_bin_p + 1));
					}
					/*data_bin_p += value_len;*/
					sentence_buffer_p += write_size;
				}
				data_bin_p += value_len;
			}
		}
		/*printf("\n");*/
		fwrite(sentence_buffer, sentence_buffer_p - sentence_buffer, 1, fp_txt);
		fprintf(fp_txt, "\n");
		/* 2nd seperator */
		fprintf(fp_txt, "######\n");
		/*fprintf(fp_txt, "translation :\n");*/
		fwrite(sentence_buffer, sentence_buffer_p - sentence_buffer, 1, fp_txt);
		fprintf(fp_txt, "\n");
		/* 3rd seperator */
		fprintf(fp_txt, "######\n");
		fprintf(fp_txt, "\n");
		fprintf(fp_txt, "\n");
		cur_section = cur_section->next;
	}
	return 0;
}

/*
 ************
 * UTILTITS *
 ************
 */

int read_file(unsigned char **data, unsigned int *len, FILE *fp)
{
	fseek(fp, 0, SEEK_END);
	*len = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	*data = (unsigned char *)malloc(sizeof(unsigned char) * (*len));
	if (*data == NULL) return -1;
	fread(*data, *len, 1, fp);
	return 0;
}

/*
 ******************
 * EXPORT ROUTINE *
 ******************
 */

int export(char *filename_txt,
		char *filename_bin,
		char *filename_tbl)
{
	int ret;
	FILE *fp_txt = NULL, *fp_bin = NULL, *fp_tbl = NULL;
	unsigned char *data_tbl = NULL;
	unsigned char *data_bin = NULL;
	size_t fp_tbl_len;
	size_t fp_bin_len;
	struct code_set *code_set = NULL;
	struct section_set *section_set = NULL;
	fp_txt = fopen(filename_txt, "wb+");
	fp_bin = fopen(filename_bin, "rb");
	fp_tbl = fopen(filename_tbl, "rb");
	if (!(fp_txt && fp_bin && fp_tbl))
	{
		goto fail;
	}
	ret = read_file(&data_tbl, &fp_tbl_len, fp_tbl);
	if (ret) goto fail;
	/*fwrite(data_tbl, fp_tbl_len, 1, stdout);*/
	code_set = parse_code_set(data_tbl, fp_tbl_len);
	if (code_set == NULL) goto fail;
	/*printf("code count = %d\n", code_set->size);*/
	/*code_set_walk(code_set);*/

	ret = read_file(&data_bin, &fp_bin_len, fp_bin);
	if (ret) goto fail;
	section_set = parse_section_set(data_bin, fp_bin_len);
	if (section_set == NULL) goto fail;
	/*printf("section_count = %d\n", section_set->size);*/

	/*section_set_walk(section_set);*/
	text_replace(fp_txt, data_bin, fp_bin_len, section_set, code_set);

	ret = 0;
	goto done;
fail:
	ret = -1;
done:
	if(fp_txt) fclose(fp_txt);
	if(fp_bin) fclose(fp_bin);
	if(fp_tbl) fclose(fp_tbl);
	if (data_tbl) free(data_tbl);
	if (data_bin) free(data_bin);
	if (code_set) code_set_destroy(code_set);
	if (section_set) section_set_destroy(section_set);
	return ret;
}

/*
 ******************
 * IMPORT ROUTINE *
 ******************
 */

	//(NO._/n #*/n .* #*/n(_*|<_*>)* #*/n/n)*

struct blocs{
	unsigned char *beg;
	unsigned char *end;
	blocs* next;
};
struct blocs* blocs_push(struct blocs* stack, unsigned char *beg, unsigned char *end){
	struct blocs* ret = (struct blocs*)malloc(sizeof(struct blocs));
	ret->beg= beg;
	ret->end= end;
	ret->next= stack;
	return ret;
}
unsigned char* getline(unsigned char* p){
	while(*p++ != '\n');
	return p;
}
struct blocs* get_blocs(unsigned char *data_txt, size_t fp_txt_len){
	unsigned char *ptxt= data_txt;
	struct blocs* ret= NULL;
	int _block=1;
	while(ptxt<data_txt+fp_txt_len){
		//skip No.
		ptxt=getline(ptxt); 
		//skip ######
		ptxt=getline(ptxt); 
		//skip upbloc
		do{
			ptxt=getline(ptxt);
		}while(!streq_n(ptxt,"######",6));
		//skip ######
		ptxt=getline(ptxt); 

		{//readbloc
			unsigned char *beg= ptxt;
			do{
				ptxt=getline(ptxt); 
			}while(!streq_n(ptxt,"######",6));
			ret=blocs_push(ret, beg, ptxt);
		}
		
		//skip ######
		ptxt=getline(ptxt); 
		//skip /n/n
		ptxt=getline(ptxt); 
		ptxt=getline(ptxt); 
		//next bloc
		_block++;
	}
	return ret;
}

void ltat_term(unsigned char** dest, unsigned char** src){
	(*src)+=2;//'<@'
	(*dest)[0]=0x1b;
	(*dest)[1]=0x40;
	(*dest)+=2;
	// [^>]*
	while(**src!='>'){
		**dest = **src;
		(*src)++;
		(*dest)++;
	}
	(*src)++;//'>'
}
void lt_term(unsigned char** dest, unsigned char** src){
	(*src)++;//'<'
	// [^>]*
	while(**src!='>'){
		**dest = codehexptou32(*src,2);
		(*src)+=2;
		(*dest)++;
	}
	(*src)++;//'>'
}
int str_term(unsigned char** dest, unsigned char** src, struct code_set *cd_set){
	struct code* cd= code_set_find_value(cd_set, (char*)*src);
	if(cd==NULL) 
		return -1;
	(*src)+=cd->len;
	if(cd->value&0x8000==0){
		(*dest)[0]=cd->value&0xff;
		(*dest)++;
	}else{
		(*dest)[1]=cd->value&0xff;
		(*dest)[0]=(cd->value>>8)&0xff;
		(*dest)+=2;
	}
	return 0;
}

unsigned char* translate_term(unsigned char* dest, struct blocs bloc, struct code_set *cd_set, char* debug_txt){
	while(bloc.beg<bloc.end){
		if(*bloc.beg=='\r'||*bloc.beg=='\n'){
			bloc.beg++;
		}else if(streq_n(bloc.beg,"<@",2)){
			ltat_term(&dest,&bloc.beg);
		}else if(streq_n(bloc.beg,"<",1)){
			lt_term(&dest,&bloc.beg);
		}else{
			//map str
			if(str_term(&dest, &bloc.beg, cd_set)< 0){
				//fail
				printf(debug_txt);
				printf("-->\n");
				char buf[128];
				memcpy(buf, bloc.beg, 128);
				buf[9]='\n';
				buf[10]='\0';
				printf(buf);
				return dest;
			}
		}
	}
	return dest;
}

//return length
int make_data(char* dest, struct blocs* bloc, int bloc_length, struct code_set *cd_set){
	char buf[256];
	char* beg= dest;
	int* header= (int*)dest;
	header[0]= bloc_length;
	dest+=4+bloc_length*4;
	int n=0;
	for(; bloc!=NULL; bloc=bloc->next){
		n++;
		header[n]= dest-beg;
		sprintf(buf, "error in No. %d  please check your input-file : *.txt *.tbl \n", n); 
		dest= (char*)translate_term((unsigned char*)dest, *bloc, cd_set, buf);
	}
	//assert
	if(n!=bloc_length) printf("error bloc_length.  you can e-mail to 0x00.pl@gmail.com or QQ:290777990 \n");
	return dest-beg;
}
struct blocs* blocs_r_head(struct blocs* head, int* out_blocks_length){
	struct blocs* temp;
	struct blocs* ret= NULL;
	*out_blocks_length= 0;
	while(head!=NULL){
		(*out_blocks_length)++;
		temp= head;
		head= head->next;
		temp->next= ret;
		ret= temp;
	}
	return ret;
}
void encode_data(char* data, int len){
	const unsigned int keyLen = 0x11;
    unsigned char key[keyLen] = 
    {
        'M', 's', 'g', 'L', 'i', 'n', 'k', 'e', 'r', 
        ' ', 'V', 'e', 'r', '1', '.', '0', '0'
    };
	for(int i=0; i<len; i++){
		data[i]= data[i]^key[i%keyLen];
	}
}
int import(char *filename_bin,
		   char *filename_txt,
		   char *filename_tbl)
{
	FILE *fp_txt = NULL, *fp_bin = NULL, *fp_tbl = NULL;
	unsigned char *data_tbl = NULL;
	unsigned char *data_txt = NULL;
	size_t fp_tbl_len;
	size_t fp_txt_len;
	int fp_bin_len=0;
	struct code_set *cd_set = NULL;
	struct blocs* bloc_head = NULL;
	int bloc_size = 0;
	int ret=-1;


	//open file
	fp_txt = fopen(filename_txt, "rb");
	fp_bin = fopen(filename_bin, "wb+");
	fp_tbl = fopen(filename_tbl, "rb");
	if (!(fp_txt && fp_bin && fp_tbl)){
		printf("no can not open or create file.\n");
		goto fail;
	}
	
	//load tbl
	ret = read_file(&data_tbl, &fp_tbl_len, fp_tbl);
	if (ret<0){
		printf("no can not read file.\n");
		goto fail;
	}
	cd_set = parse_code_set(data_tbl, fp_tbl_len);
	if (cd_set == NULL) goto fail;

	//load txt
	ret = read_file(&data_txt, &fp_txt_len, fp_txt);
	if (ret<0){
		printf("no can not read file.\n");
		goto fail;
	}
	bloc_head= get_blocs(data_txt, fp_txt_len);	
	if (bloc_head==NULL){
		printf("no can not get bloc.\n");
		goto fail;
	}
	bloc_head= blocs_r_head(bloc_head, &bloc_size);

	//write bin
	////get buff len
	char* buf= (char*)malloc(fp_txt_len*2);
	fp_bin_len= make_data(buf, bloc_head, bloc_size, cd_set);
	////encode
	encode_data(buf, fp_bin_len);
	fwrite(buf, 1, fp_bin_len, fp_bin);
	free(buf);
	goto done;


fail:
	ret = -1;
done:
	if(fp_txt) fclose(fp_txt);
	if(fp_bin) fclose(fp_bin);
	if(fp_tbl) fclose(fp_tbl);
	if(cd_set) code_set_destroy(cd_set);
	return ret;
}

int XX_main(int argc, char *argv[])
{
	//import("output.bin", "a.txt", "a.tbl");

	//printf("--end--\n");
	//system("pause");
	//return 0;

	int ret;
	if ((argc != 5) ||
			!((argv[1][0] == 'i')|| 
			(argv[1][0] == 'e')))
	{
		printf("pokenobu text export and import tool\n");
		printf("usage : %s e <output.txt> <input.bin> <table.tbl>\n", argv[0]);
		printf("        %s i <output.bin> <input.txt> <table.tbl>\n", argv[0]);
		exit(1);
	}

	if (argv[1][0] == 'e')
	{
		ret = export(argv[2], argv[3], argv[4]);
	} 
	else if (argv[1][0] == 'i')
	{
		ret = import(argv[2], argv[3], argv[4]);
	}
	/*ret = convert(argv[1], argv[1], argv[2]"a.txt", "a.bin", "a.tbl");*/
	if (ret) printf("error\n");
	return 0;
}

