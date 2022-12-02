struct Keyframe
{
	real32 duration;
	Sprite* sprite;
};

struct Animation
{
	Keyframe frame[30];
	uint32 frameCount;
	uint32 currentFrame;

	real32 timeSinceLastFrame;

	bool loop;
};

void PlayAnimation(Animation anim)
{
		if (anim.timeSinceLastFrame >= anim.frame[anim.currentFrame].duration)
		{
			anim.currentFrame++;
			if (anim.currentFrame >= anim.frameCount)
			{
				if (anim.loop)
				{
					anim.currentFrame = 0;
				}
				else
				{
					anim.currentFrame--;
				}
			}
			anim.timeSinceLastFrame = 0.0f;
		}
		else
		{
			anim.timeSinceLastFrame += 1 * DeltaTime;
		}
}

void StartAnimation(Animation anim)
{
	anim.currentFrame = 0;
	PlayAnimation(anim);
}

Sprite* GetFrame(Animation anim)
{
	return anim.frame[anim.currentFrame].sprite;
}


