enum TokenType 
{
	TokenType_LeftParen,
	TokenType_RightParen,
	TokenType_Integer,
	TokenType_Pound,
	TokenType_Identifier
};

struct Token
{
	TokenType type;
	char* start;
	int32 length;
};

void ReadLevel()
{
	DynamicArray<Token> tokens = MakeDynamicArray<Token>(&Game->frameMem, 100);
}