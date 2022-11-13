enum LevelTokenType
{
	LevelTokenType_LeftParen,
	LevelTokenType_RightParen,
	LevelTokenType_Integer,
	LevelTokenType_Pound,
	LevelTokenType_Identifier,
	LevelTokenType_Dollar
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

void ReadLevel(EntityManager* em, char* filePath)
{
	DynamicArray<Token> tokens = MakeDynamicArray<Token>(&Game->frameMem, 100);
	MemoryArena arena = {};
	//AllocateMemoryArena(&arena, Megabytes(8));

	char intArray[6];

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
			if (strncmp("footmen", t.start, t.length) == 0)
			{
				while (true)
				{
					EntityTypeBuffer* buffer = &em->buffers[EntityType_Footman];
					tokenIndex++;
					if (t.type == LevelTokenType_LeftParen)
					{
						EntityHandle footmanHandle = AddEntity(em, EntityType_Footman);
						EntityInfo* info = &em->entities[footmanHandle.id];
						Footman* f = (Footman*)&buffer[info->indexInBuffer];
						tokenIndex++;

						for (int i = 0; i < 2; i++) //set position
						{
							if (t.type == LevelTokenType_Integer)
							{
								memcpy(intArray, t.start, t.length);
								switch (i)
								{
								case 0:
									f->position.x = atoi(intArray);
									break;
								case 1:
									f->position.y = atoi(intArray);
									break;
								}
								tokenIndex++;
							}
						}

						for (int i = 0; i < 2; i++) //set direction
						{
							if (t.type == LevelTokenType_Integer)
							{
								memcpy(intArray, t.start, t.length);
								switch (i)
								{
								case 0:
									f->direction.x = atoi(intArray);
									break;
								case 1:
									f->direction.y = atoi(intArray);
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
					if (t.type == LevelTokenType_Dollar)
					{
						break;
					}
				}
			}
			if (strncmp("crossbowmen", t.start, t.length) == 0)
			{
				while (true)
				{
					if (t.type == LevelTokenType_LeftParen)
					{
						for (int i = 0; i < 2; i++)
						{

						}

						for (int i = 0; i < 2; i++)
						{

						}
					}

					if (t.type == LevelTokenType_RightParen)
					{

					}
					if (t.type == LevelTokenType_Dollar)
					{

					}
				}
			}
			if (strncmp("knights", t.start, t.length) == 0)
			{
				while (true)
				{
					if (t.type == LevelTokenType_LeftParen)
					{
						for (int i = 0; i < 2; i++)
						{

						}

						for (int i = 0; i < 2; i++)
						{

						}
					}

					if (t.type == LevelTokenType_RightParen)
					{

					}
					if (t.type == LevelTokenType_Dollar)
					{

					}
				}
			}
			if (strncmp("wizards", t.start, t.length) == 0)
			{
				while (true)
				{
					if (t.type == LevelTokenType_LeftParen)
					{
						for (int i = 0; i < 2; i++)
						{

						}

						for (int i = 0; i < 2; i++)
						{

						}
					}

					if (t.type == LevelTokenType_RightParen)
					{

					}
					if (t.type == LevelTokenType_Dollar)
					{

					}
				}
			}
		}
		
		tokenIndex++;
	}

	CloseFile(&file);
}