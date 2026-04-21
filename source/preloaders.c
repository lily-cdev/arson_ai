#include <sublinker.h>
#define byte unsigned char
#define TRANSPARENT 254
#define BITSPERPIXEL 24
#define HEADERSIZE 54

struct ai_textures Textures = { };

int32_t Read_Byte4(byte* Data, uint64_t Position) {
	int32_t Yield = 0;
	for (uint8_t C1 = 0; C1 < 4; C1++) {
		Yield |= Data[C1 + Position] << (C1 * 8);
	}
	return Yield;
}

SDL_Surface* Load_BMP(const char* Path) {
	FILE* Image = fopen(Path, "rb");
	if (Image == NULL) {
		return NULL;
	}
	fseek(Image, 0, SEEK_END);
	uint64_t Length = ftell(Image);
	rewind(Image);
	byte* Data = calloc(Length, sizeof(byte));
	fread(Data, sizeof(byte), Length, Image);
	if (Data[0] != 'B' || Data[1] != 'M') {
		free(Data);
		return NULL;
	}
	int Width = Read_Byte4(Data, 18);
	int Height = Read_Byte4(Data, 22);
	SDL_Surface* Yield = SDL_CreateSurface(Width, Height, SDL_PIXELFORMAT_RGBA8888);
	SDL_LockSurface(Yield);
	uint32_t* Pixels = (uint32_t*)Yield->pixels;
	uint64_t Row_Size = ceil(Width * BITSPERPIXEL * 0.03125f) * 4;
	uint64_t Index = HEADERSIZE;
	uint8_t Opacity;
	const SDL_PixelFormatDetails* Details = SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_RGBA8888);
	for (int Y = 0; Y < Height; Y++) {
		for (int X = 0; X < Width; X++) {
			Opacity = SDL_ALPHA_OPAQUE;
			if (Data[Index + 2] == TRANSPARENT && Data[Index + 1] == TRANSPARENT && Data[Index] ==
                TRANSPARENT) {
				Opacity = SDL_ALPHA_TRANSPARENT;
			}
			Pixels[((Height - Y - 1) * Width) + X] = SDL_MapRGBA(Details, NULL, Data[Index + 2], Data[
                Index + 1], Data[Index], Opacity);
			Index += 3;
		}
		Index += Row_Size - (Width * 3);
	}
	SDL_UnlockSurface(Yield);
	fclose(Image);
	free(Data);
	return Yield;
}

void Load_Modular(const char* Path, int Size, SDL_Texture* Yield[Size]) {
	SDL_Surface* Root = Load_BMP(Path);
    for (int C1 = 0; C1 < Size; C1++) {
        SDL_Surface* Subsurface = SDL_CreateSurface(AI_TILESIZE, AI_TILESIZE, SDL_PIXELFORMAT_RGBA8888);
        SDL_Rect Source = { C1 * AI_TILESIZE, 0, AI_TILESIZE, AI_TILESIZE };
        SDL_BlitSurface(Root, &Source, Subsurface, NULL);
        Yield[C1] = SDL_CreateTextureFromSurface(Core.Renderer, Subsurface);
        SDL_DestroySurface(Subsurface);
    }
    SDL_DestroySurface(Root);
}

void Load_All() {
    Load_Modular("assets/path.bmp", 5, Textures.Path);
	SDL_Surface* Carrier = Load_BMP("assets/hut_4x4.bmp");
	Textures.Hut = SDL_CreateTextureFromSurface(Core.Renderer, Carrier);
	SDL_DestroySurface(Carrier);
	Carrier = Load_BMP("assets/barn_6x9.bmp");
	Textures.Barn = SDL_CreateTextureFromSurface(Core.Renderer, Carrier);
	SDL_DestroySurface(Carrier);
	Carrier = Load_BMP("assets/well_1x1.bmp");
	Textures.Well = SDL_CreateTextureFromSurface(Core.Renderer, Carrier);
	SDL_DestroySurface(Carrier);
	Carrier = Load_BMP("assets/hut_5x7.bmp");
	Textures.L_Hut = SDL_CreateTextureFromSurface(Core.Renderer, Carrier);
	SDL_DestroySurface(Carrier);
	Carrier = Load_BMP("assets/clayhut_5x5.bmp");
	Textures.C_Hut = SDL_CreateTextureFromSurface(Core.Renderer, Carrier);
	SDL_DestroySurface(Carrier);
	Carrier = Load_BMP("assets/tank.bmp");
	Textures.Tank = SDL_CreateTextureFromSurface(Core.Renderer, Carrier);
	SDL_DestroySurface(Carrier);
}

void Free_All() {
    for (int C1 = 0; C1 < 5; C1++) {
        SDL_DestroyTexture(Textures.Path[C1]);
    }
	for (int C1 = 0; C1 < 8; C1++) {
		SDL_DestroyTexture(Textures.Fire[C1]);
	}
	SDL_DestroyTexture(Textures.Hut);
	SDL_DestroyTexture(Textures.Barn);
	SDL_DestroyTexture(Textures.Well);
	SDL_DestroyTexture(Textures.L_Hut);
	SDL_DestroyTexture(Textures.C_Hut);
	SDL_DestroyTexture(Textures.Tank);
	SDL_DestroyTexture(Core.Noise);
}