#pragma once

#include <emuframework/EmuSystem.hh>
#include <emuframework/Option.hh>
#include <main/Palette.hh>
#include <gambatte.h>
#include <libgambatte/src/video/lcddef.h>
#include <resample/resampler.h>
#include <imagine/fs/FS.hh>
#include <memory>

namespace EmuEx
{

enum
{
	CFGKEY_GB_PAL_IDX = 270, CFGKEY_REPORT_AS_GBA = 271,
	CFGKEY_FULL_GBC_SATURATION = 272, CFGKEY_AUDIO_RESAMPLER = 273,
	CFGKEY_USE_BUILTIN_GB_PAL = 274, CFGKEY_RENDER_PIXEL_FORMAT_UNUSED = 275
};

constexpr unsigned COLOR_CONVERSION_SATURATED_BIT = bit(0);
constexpr unsigned COLOR_CONVERSION_BGR_BIT = bit(1);

class GbcInput final : public gambatte::InputGetter
{
public:
	unsigned bits{};

	constexpr GbcInput() = default;
	constexpr unsigned operator()() final { return bits; }
};

class GbcSystem final: public EmuSystem
{
public:
	gambatte::GB gbEmu{};
	GbcInput gbcInput{};
	std::unique_ptr<Resampler> resampler{};
	const GBPalette *gameBuiltinPalette{};
	uint64_t totalSamples{};
	uint32_t totalFrames{};
	uint8_t activeResampler = 1;
	bool useBgrOrder{};
	alignas(8) uint_least32_t frameBuffer[gambatte::lcd_hres * gambatte::lcd_vres];
	Byte1Option optionGBPal{CFGKEY_GB_PAL_IDX, 0, 0, optionIsValidWithMax<gbNumPalettes-1>};
	Byte1Option optionUseBuiltinGBPalette{CFGKEY_USE_BUILTIN_GB_PAL, 1};
	Byte1Option optionReportAsGba{CFGKEY_REPORT_AS_GBA, 0};
	Byte1Option optionAudioResampler{CFGKEY_AUDIO_RESAMPLER, 1};
	Byte1Option optionFullGbcSaturation{CFGKEY_FULL_GBC_SATURATION, 0};

	GbcSystem(ApplicationContext ctx):
		EmuSystem{ctx}
	{
		gbEmu.setInputGetter(&gbcInput);
		gbEmu.setStreamDelegates(
			[ctx](std::string_view basePath, std::string_view filename) -> IG::IFStream
			{
				return {ctx.openFileUri(FS::uriString(basePath, filename), IOAccessHint::ALL, OpenFlagsMask::TEST)};
			},
			[ctx](std::string_view basePath, std::string_view filename) -> IG::OFStream
			{
				return {ctx.openFileUri(FS::uriString(basePath, filename), OpenFlagsMask::NEW | OpenFlagsMask::TEST)};
			});
	}
	void applyGBPalette();
	void applyCheats();
	void refreshPalettes();

	// required API functions
	void loadContent(IO &, EmuSystemCreateParams, OnLoadProgressDelegate);
	[[gnu::hot]] void runFrame(EmuSystemTaskContext task, EmuVideo *video, EmuAudio *audio);
	FS::FileString stateFilename(int slot, std::string_view name) const;
	void loadState(EmuApp &, CStringView uri);
	void saveState(CStringView path);
	bool readConfig(ConfigType, MapIO &, unsigned key, size_t readSize);
	void writeConfig(ConfigType, FileIO &);
	void reset(EmuApp &, ResetMode mode);
	void clearInputBuffers(EmuInputView &view);
	void handleInputAction(EmuApp *, InputAction);
	unsigned translateInputAction(unsigned input, bool &turbo);
	VController::Map vControllerMap(int player);
	void configAudioRate(FloatSeconds frameTime, int rate);
	static std::span<const AspectRatioInfo> aspectRatioInfos();

	// optional API functions
	void onFlushBackupMemory(BackupMemoryDirtyFlags);
	void savePathChanged();
	void closeSystem();
	void onOptionsLoaded();
	bool resetSessionOptions(EmuApp &);
	bool onVideoRenderFormatChange(EmuVideo &, IG::PixelFormat);
	void renderFramebuffer(EmuVideo &);
protected:
	uint_least32_t makeOutputColor(uint_least32_t rgb888) const;
	size_t runUntilVideoFrame(gambatte::uint_least32_t *videoBuf, std::ptrdiff_t pitch,
		EmuAudio *audio, gambatte::VideoFrameDelegate videoFrameCallback);
	void renderVideo(const EmuSystemTaskContext &taskCtx, EmuVideo &video);
	void updateColorConversionFlags();
};

using MainSystem = GbcSystem;

}