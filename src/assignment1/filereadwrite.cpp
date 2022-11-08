enum LevelTokenType
{
	LevelTokenType_LeftParen,
	LevelTokenType_RightParen,
	LevelTokenType_Integer,
	LevelTokenType_Pound,
	LevelTokenType_Identifier
};

struct Token
{
	LevelTokenType type;
	char* start;
	int32 length;
};

bool IsLetter(char c)
{
	bool isLetter = false;
	if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122))
	{
		isLetter = true;
	}

	return isLetter;
}

bool IsNumber(char c)
{
	bool isNumber = false;

	if (c >= 48 && c <= 57)
	{
		isNumber = true;
	}

	return isNumber;
}

void ReadLevel(char* filePath)
{
	DynamicArray<Token> tokens = MakeDynamicArray<Token>(&Game->frameMem, 100);
	MemoryArena arena = {};
	//AllocateMemoryArena(&arena, Megabytes(8));

	FileHandle file = {};

	if (!OpenFileForRead(filePath, &file, &arena))
	{
		return;
	}
	//tokenize
	while (file.offset < file.size)
	{
		char c = ReadChar(&file);

		Token t = {};
		t.start = (char*)&file.data[file.offset];

		if (c == '(')
		{
			t.length = 1;
			t.type = LevelTokenType_LeftParen;
		}

		if (c == ')')
		{
			t.length = 1;
			t.type = LevelTokenType_RightParen;
		}

		if (c == '#')
		{
			t.length = 1;
			t.type = LevelTokenType_Pound;
		}

		while (IsLetter(c))
		{
			t.type = LevelTokenType_Identifier;
			t.length++;
			c = ReadChar(&file);
		}

		while (IsNumber(c))
		{
			t.type = LevelTokenType_Integer;
			t.length++;
			c = ReadChar(&file);
		}

		PushBack(&tokens, t);
	}
	//parse
	int32 tokenIndex = 0;
	while (tokenIndex < tokens.count)
	{
		Token t = tokens[tokenIndex];

		if (t.type == LevelTokenType_Pound)
		{

		}
		
		if (strncmp("footmen", t.start, t.length) == 0)
		{
			EntityTypeBuffer* buffer = &em->buffers[EntityType_Footman];
			tokenIndex++;
			if (t.type == LevelTokenType_LeftParen)
			{
				EntityHandle* footmanHandle = AddEntity(&em, EntityType_Footman);
				EntityInfo* info = &em->entities[footmanHandle->id];
				Footman* f = (Footman*)&buffer[info->indexInBuffer];
				tokenIndex++;
				for (int i = 0; i < 4; i++)
				{
					if (t.type == LevelTokenType_Integer)
					{
						switch (i)
						{
							case 0;
								f->position.x = ;
								break;
							case 1:
								f->position.y = ;
								break;
							case 2:
								f->direction.x = ;
								break;
							case 3:
								f->direction.y = ;
								break;
						}
						tokenIndex++;
					}
				}

			}
			if (t.type == LevelTokenType_RightParen)
			{
				tokenIndex++;
			}
		}
		if (strncmp("crossbowmen", t.start, t.length) == 0)
		{

		}
		if (strncmp("knights", t.start, t.length) == 0)
		{

		}
		if (strncmp("wizards", t.start, t.length) == 0)
		{

		}
		tokenIndex++;
	}

	CloseFile(&file);
}