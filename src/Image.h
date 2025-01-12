#ifndef __NODE_IMAGE_H__
#define __NODE_IMAGE_H__

#include <node_api.h>
#include <stdlib.h>
#include <stdio.h>

enum ImageType
{
	TYPE_PNG = 1,
	TYPE_JPEG,
	TYPE_GIF,
	TYPE_BMP,
	TYPE_RAW,
	TYPE_WEBP,
};

enum ImageState
{
	FAIL,
	SUCCESS,
};

struct Pixel
{
	uint8_t R;
	uint8_t G;
	uint8_t B;
	uint8_t A;

	void Merge(struct Pixel *pixel);
};

enum PixelArrayType {
	EMPTY,
	ALPHA,
	SOLID,
};

struct PixelArray
{
	Pixel **data;
	size_t width;
	size_t height;
	PixelArrayType type;

	int32_t Size()
	{
		return (height * sizeof(Pixel **)) + (width * height * sizeof(Pixel));
	}

	// Memory
	ImageState Malloc(size_t w, size_t h);

	ImageState CopyFrom(struct PixelArray *src, size_t x, size_t y, size_t w, size_t h);

	void Free();

	// Draw
	void Draw(struct PixelArray *src, size_t x, size_t y);

	void Fill(Pixel *color);

	// Transform
	ImageState SetWidth(size_t w);

	ImageState SetHeight(size_t h);

	ImageState Resize(size_t w, size_t h, const char *filter);

	ImageState Rotate(size_t deg);

	void DetectTransparent();
};

struct ImageData
{
	uint8_t *data;
	unsigned long length;
	unsigned long position;
};

struct ImageConfig
{
	char *data;
	unsigned long length;
};

typedef ImageState (*ImageEncoder)(PixelArray *input, ImageData *output, ImageConfig *config);

typedef ImageState (*ImageDecoder)(PixelArray *output, ImageData *input);

struct ImageCodec
{
	ImageType type;
	ImageEncoder encoder;
	ImageDecoder decoder;
	struct ImageCodec *next;
};

#ifdef HAVE_PNG
ImageState encodePng(PixelArray * input, ImageData * output, ImageConfig * config);
ImageState decodePng(PixelArray * output, ImageData * input);
#endif

#ifdef HAVE_JPEG
ImageState encodeJpeg(PixelArray * input, ImageData * output, ImageConfig * config);
ImageState decodeJpeg(PixelArray * output, ImageData * input);
#endif

#ifdef HAVE_GIF
ImageState encodeGif(PixelArray * input, ImageData * output, ImageConfig * config);
ImageState decodeGif(PixelArray * output, ImageData * input);
#endif

#ifdef HAVE_BMP
ImageState encodeBmp(PixelArray * input, ImageData * output, ImageConfig * config);
ImageState decodeBmp(PixelArray * output, ImageData * input);
#endif

#ifdef HAVE_RAW
ImageState encodeRaw(PixelArray * input, ImageData * output, ImageConfig * config);
ImageState decodeRaw(PixelArray * output, ImageData * input);
#endif

#ifdef HAVE_WEBP
ImageState encodeWebp(PixelArray * input, ImageData * output, ImageConfig * config);
ImageState decodeWebp(PixelArray * output, ImageData * input);
#endif

class Image
{
public:
	// static v8::Persistent<v8::Function> constructor;
	static napi_ref constructor;
	// static void Init(v8::Local<v8::Object> exports);
	static napi_value Init(napi_env env, napi_value exports);
	static void Destructor(napi_env env, void *nativeObject, void *finalize_hint);
	static napi_value New(napi_env env, napi_callback_info info);

	// Error Handle
	static void setError(const char *err);

	static napi_value getError(napi_env, napi_callback_info info);

	static bool isError();

	// Size Limit
	static size_t maxWidth, maxHeight;

	static napi_value GetMaxWidth(napi_env env, napi_callback_info info);
	static napi_value SetMaxWidth(napi_env env, napi_callback_info info);

	static napi_value GetMaxHeight(napi_env env, napi_callback_info info);
	static napi_value SetMaxHeight(napi_env env, napi_callback_info info);

	// Memory
	static size_t usedMemory;

	static napi_value GetUsedMemory(napi_env env, napi_callback_info info);
	static napi_value GC(napi_env env, napi_callback_info info);

	// Image constructor
	// static void New(const v8::FunctionCallbackInfo<v8::Value> &args);

	// Image.prototype
	static napi_value GetWidth(napi_env env, napi_callback_info info);
	static napi_value SetWidth(napi_env env, napi_callback_info info);

	static napi_value GetHeight(napi_env env, napi_callback_info info);
	static napi_value SetHeight(napi_env env, napi_callback_info info);

	static napi_value GetTransparent(napi_env env, napi_callback_info info);

	static napi_value Resize(napi_env env, napi_callback_info info);

	static napi_value Rotate(napi_env env, napi_callback_info info);

	static napi_value FillColor(napi_env env, napi_callback_info info);

	static napi_value LoadFromBuffer(napi_env env, napi_callback_info info);

	static napi_value ToBuffer(napi_env env, napi_callback_info info);

	static napi_value CopyFromImage(napi_env env, napi_callback_info info);

	static napi_value DrawImage(napi_env env, napi_callback_info info);

private:
	static const char *error;
	static int errno;

	static ImageCodec *codecs;

	static void regCodec(ImageDecoder decoder, ImageEncoder encoder, ImageType type);

	static void regAllCodecs()
	{
		codecs = NULL;

		#ifdef HAVE_WEBP
		regCodec(decodeWebp, encodeWebp, TYPE_WEBP);
		#endif

		#ifdef HAVE_RAW
		regCodec(decodeRaw, encodeRaw, TYPE_RAW);
		#endif

		#ifdef HAVE_BMP
		regCodec(decodeBmp, encodeBmp, TYPE_BMP);
		#endif

		#ifdef HAVE_GIF
		regCodec(decodeGif, encodeGif, TYPE_GIF);
		#endif

		#ifdef HAVE_JPEG
		regCodec(decodeJpeg, encodeJpeg, TYPE_JPEG);
		#endif

		#ifdef HAVE_PNG
		regCodec(decodePng, encodePng, TYPE_PNG);
		#endif
	}

	PixelArray *pixels;

	Image();

	~Image();

	// napi env & wrapper
	napi_env env_;
	napi_ref wrapper_;
};

#endif
