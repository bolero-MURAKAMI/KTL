#pragma once

#include <cstddef>
#include <string>
#include <vector>
#include <map>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <sprig/external/windows.hpp>
#include <sprig/exception.hpp>
#include <sprig/str_cast.hpp>
#include <sprig/section.hpp>
#include <sprig/libpng/png.hpp>
#include <sprig/krkr/tjs.hpp>
#include <sprig/krkr/exception.hpp>
#include <ktl/error.hpp>

#define KTL_WARNING_SECTION SPRIG_KRKR_TJS_W("KTL:WindowBase:warning")
#define KTL_ERROR_SECTION SPRIG_KRKR_TJS_W("KTL:WindowBase:error")

namespace ktl {
	//
	// WindowUtils
	//
	class WindowUtils {
	public:
		typedef std::size_t size_type;
		typedef boost::int_fast32_t int_type;
		typedef std::basic_string<TCHAR> string_type;
		typedef tjs_uint8 byte_type;
		typedef std::vector<byte_type> bits_type;
	public:
		class end_paint {
		private:
			HWND const window_handle_;
			PAINTSTRUCT const* const ps_;
		public:
			end_paint(
				HWND window_handle,
				PAINTSTRUCT const* ps
				)
				: window_handle_(window_handle)
				, ps_(ps)
			{}
			void operator()(HDC dc_handle) const {
				::ReleaseDC(window_handle_, dc_handle);
				::EndPaint(window_handle_, ps_);
			}
		};
		class release_dc {
		private:
			HWND const window_handle_;
		public:
			release_dc(HWND window_handle)
				: window_handle_(window_handle)
			{}
			void operator()(HDC dc_handle) const {
				::ReleaseDC(window_handle_, dc_handle);
			}
		};
		class delete_dc {
		public:
			delete_dc() {}
			void operator()(HDC dc_handle) const {
				::DeleteDC(dc_handle);
			}
		};
		class select_object {
		private:
			HDC const dc_handle_;
		public:
			select_object(HDC dc_handle)
				: dc_handle_(dc_handle)
			{}
			void operator()(HGDIOBJ gdi_object_handle) const {
				::SelectObject(dc_handle_, gdi_object_handle);
			}
		};
	public:
		static bool isPNG(byte_type const* data, size_type size) {
			return sprig::png::png_sig_check(data, size);
		}
		static void updateWindow(HWND window_handle) {
			if (window_handle) {
				if (!::UpdateWindow(window_handle)) {
					KTL_ERROR(
						KTL_ERROR_SECTION,
						SPRIG_KRKR_TJS_W("ウインドウの再描画に失敗しました"),
						sprig::krkr::win32_error
						);
					return;
				}
			}
		}
		static bool loadImagesFromPNG(
			bits_type& bits,
			BITMAPINFO& bitmap_info,
			sprig::png::png_pixels_order pixels_order,
			byte_type const* data,
			size_type size
			)
		{
			sprig::png::png_reader reader;
			sprig::png::IHDR ihdr;
			if (SPRIG_PNG_SETJMP(reader.png())) {
				KTL_ERROR(
					KTL_ERROR_SECTION,
					SPRIG_KRKR_TJS_W("libpngで内部エラーが発生しました"),
					sprig::krkr::lib_error
					);
				return false;
			}
			bits.clear();
			reader.read_all(data, ihdr, bits, pixels_order, true);
			bitmap_info.bmiHeader.biWidth = ihdr.width;
			bitmap_info.bmiHeader.biHeight = ihdr.height;
			return true;
		}
		static bool loadImages(
			bits_type& bits,
			BITMAPINFO& bitmap_info,
			sprig::png::png_pixels_order pixels_order,
			byte_type const* data,
			size_type size
			)
		{
			if (WindowUtils::isPNG(data, size)) {
				if (!loadImagesFromPNG(bits, bitmap_info, pixels_order, data, size)) {
					KTL_ERROR(
						KTL_ERROR_SECTION,
						SPRIG_KRKR_TJS_W("PNGのロードに失敗しました"),
						sprig::krkr::internal_error
						);
					return false;
				}
			} else {
				KTL_ERROR(
					KTL_ERROR_SECTION,
					SPRIG_KRKR_TJS_W("非対応の画像形式です"),
					sprig::krkr::internal_error
					);
				return false;
			}
			return true;
		}
	};
	//
	// WindowBase
	//
	class WindowBase {
	public:
		typedef WindowUtils::size_type size_type;
		typedef WindowUtils::int_type int_type;
		typedef WindowUtils::string_type string_type;
		typedef WindowUtils::byte_type byte_type;
		typedef WindowUtils::bits_type bits_type;
	protected:
		HWND window_handle_;
		WNDCLASSEX window_class_;
		ATOM atom_;
		int_type x_;
		int_type y_;
		size_type width_;
		size_type height_;
		string_type identity_;
		bool visible_;
		bool topMost_;
		BITMAPINFO bitmap_info_;
		bits_type bits_;
	protected:
		WindowBase()
			: window_handle_(0)
			, atom_(0)
			, x_(0)
			, y_(0)
			, width_(0)
			, height_(0)
			, identity_()
			, visible_(false)
			, topMost_(false)
		{
			window_class_.cbSize = sizeof(WNDCLASSEX);				// 構造体のサイズ
			window_class_.style = CS_VREDRAW | CS_HREDRAW;			// クラススタイル
			window_class_.lpfnWndProc = 0;							// ウィンドウプロシージャ
			window_class_.cbClsExtra = 0;							// 補足メモリブロックのサイズ
			window_class_.cbWndExtra = 0;							// 補足メモリブロックのサイズ
			window_class_.hInstance = ::GetModuleHandle(0);			// インスタンス
			window_class_.hIcon = ::LoadIcon(0, IDI_APPLICATION);	// アイコン
			window_class_.hCursor = ::LoadCursor(0, IDC_ARROW);		// カーソル
			window_class_.hbrBackground = (HBRUSH)COLOR_WINDOW + 1;	// 背景色
			window_class_.lpszMenuName = 0;							// メニュー名
			window_class_.lpszClassName = 0;						// クラス名
			window_class_.hIconSm = 0;								// 小さいアイコン
			{
				BITMAPINFOHEADER& header = bitmap_info_.bmiHeader;
				header.biSize = sizeof(BITMAPINFOHEADER);
				header.biWidth = 0;
				header.biHeight = 0;
				header.biPlanes = 1;
				header.biBitCount = 24;
				header.biCompression = BI_RGB;
				header.biSizeImage = 0;
				header.biXPelsPerMeter = 0;
				header.biYPelsPerMeter = 0;
				header.biClrUsed = 0;
				header.biClrImportant = 0;
			}
			{
				RGBQUAD color = { 0 };
				bitmap_info_.bmiColors[0] = color;
			}
		}
		void dispose() {
			if (window_handle_) {
				if (!::DestroyWindow(window_handle_)) {
					KTL_ERROR_NOTHROW(
						KTL_ERROR_SECTION,
						SPRIG_KRKR_TJS_W("ウインドウの破棄に失敗しました")
						);
				}
				window_handle_ = 0;
			}
			if (atom_) {
				if (!::UnregisterClass(reinterpret_cast<LPCTSTR>(MAKELONG(atom_, 0)), 0)) {
					KTL_ERROR_NOTHROW(
						KTL_ERROR_SECTION,
						SPRIG_KRKR_TJS_W("ウインドウクラスの登録解除に失敗しました")
						);
				}
				atom_ = 0;
			}
		}
		void initialize(
			DWORD ex_style,
			int x,
			int y,
			int width,
			int height,
			string_type const& identity = string_type()
			)
		{
			if (window_handle_) {
				KTL_ERROR(
					KTL_ERROR_SECTION,
					SPRIG_KRKR_TJS_W("既に初期化されています"),
					sprig::krkr::internal_error
					);
				return;
			}

			if (x != CW_USEDEFAULT) {
				x_ = x;
				y_ = y;
			}
			if (width != CW_USEDEFAULT) {
				width_ = width;
				height_ = height;
			}
			identity_ = !identity.empty()
				? identity
				: sprig::str_cast<string_type>(boost::uuids::random_generator()())
				;
			window_class_.lpszClassName = identity_.c_str();
			if (!(atom_ = ::RegisterClassEx(&window_class_))) {
				KTL_ERROR(
					KTL_ERROR_SECTION,
					SPRIG_KRKR_TJS_W("ウインドウクラスの登録に失敗しました"),
					sprig::krkr::win32_error
					);
				return;
			}
			window_handle_ = ::CreateWindowEx(
				ex_style,										// 拡張ウィンドウスタイル
				reinterpret_cast<LPCTSTR>(MAKELONG(atom_, 0)),	// クラス名
				window_class_.lpszClassName,					// ウィンドウタイトル
				WS_POPUP,										// ウィンドウスタイル
				x,												// ウィンドウ位置
				y,												// ウィンドウ位置
				width,											// ウィンドウサイズ
				height,											// ウィンドウサイズ
				0,												// 親ウィンドウ
				0,												// メニュー
				window_class_.hInstance,						// インスタンス
				0												// ウィンドウ作成データのアドレス
				);
			if (!window_handle_) {
				KTL_ERROR(
					KTL_ERROR_SECTION,
					SPRIG_KRKR_TJS_W("ウインドウの作成に失敗しました"),
					sprig::krkr::win32_error
					);
				return;
			}
		}
		bool loadImages(sprig::png::png_pixels_order pixels_order, byte_type const* data, size_type size) {
			return WindowUtils::loadImages(bits_, bitmap_info_, pixels_order, data, size);
		}
	public:
		bool getVisible() const {
			return visible_;
		}
		void setVisible(bool v) {
			if (window_handle_) {
				visible_ = v;
				::ShowWindow(
					window_handle_,
					visible_
						? SW_SHOW
						: SW_HIDE
					);
				WindowUtils::updateWindow(window_handle_);
			}
		}
		bool getTopMost() const {
			return topMost_;
		}
		void setTopMost(bool v) {
			if (window_handle_) {
				topMost_ = v;
				if (!::SetWindowPos(
					window_handle_,
					topMost_
						? HWND_TOPMOST
						: HWND_NOTOPMOST
						,
					x_,
					y_,
					width_,
					height_,
					SWP_NOMOVE | SWP_NOSIZE
					))
				{
					KTL_ERROR(
						KTL_ERROR_SECTION,
						SPRIG_KRKR_TJS_W("ウインドウのZオーダー変更に失敗しました"),
						sprig::krkr::internal_error
						);
					return;
				}
				WindowUtils::updateWindow(window_handle_);
			}
		}
		size_type getImageWidth() const {
			return bitmap_info_.bmiHeader.biWidth;
		}
		size_type getImageHeight() const {
			return bitmap_info_.bmiHeader.biHeight;
		}
		int_type getLeft() const {
			return x_;
		}
		int_type getTop() const {
			return y_;
		}
		size_type getWidth() const {
			return width_;
		}
		size_type getHeight() const {
			return height_;
		}
		string_type const& getIdentity() const {
			return identity_;
		}
		bool initialized() const {
			return window_handle_ != 0;
		}
	};
}	// namespace ktl

#undef KTL_WARNING_SECTION
#undef KTL_ERROR_SECTION

#define KTL_WARNING_SECTION SPRIG_KRKR_TJS_W("KTL:SplashWindow:warning")
#define KTL_ERROR_SECTION SPRIG_KRKR_TJS_W("KTL:SplashWindow:error")

namespace ktl {
	//
	// Splash
	//
	class Splash
		: public WindowBase
	{
	public:
		typedef WindowBase::size_type size_type;
		typedef WindowBase::int_type int_type;
		typedef WindowBase::string_type string_type;
		typedef WindowBase::byte_type byte_type;
		typedef WindowBase::bits_type bits_type;
		typedef std::map<HWND, Splash*> instance_map_type;
	private:
		static instance_map_type instance_map_;
	private:
		static LRESULT CALLBACK windowProcedure(HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param) {
			switch (message) {
			case WM_PAINT:
				if (instance_map_.find(window_handle) != instance_map_.end()) {
					Splash* instance = instance_map_[window_handle];
					PAINTSTRUCT ps = { 0 };
					boost::shared_ptr<boost::remove_pointer<HDC>::type> dc_handle(
						::BeginPaint(window_handle, &ps),
						WindowUtils::end_paint(window_handle, &ps)
						);
					if (!dc_handle) {
						KTL_ERROR(
							KTL_ERROR_SECTION,
							SPRIG_KRKR_TJS_W("描画の開始に失敗しました"),
							sprig::krkr::win32_error
							);
					}
					if (!instance->bits_.empty()) {
						if (::StretchDIBits(
							boost::get_pointer(dc_handle),
							0,
							0,
							instance->width_,
							instance->height_,
							0,
							0,
							instance->bitmap_info_.bmiHeader.biWidth,
							instance->bitmap_info_.bmiHeader.biHeight,
							&instance->bits_[0],
							&instance->bitmap_info_,
							DIB_RGB_COLORS,
							SRCCOPY
							) == GDI_ERROR)
						{
							KTL_ERROR(
								KTL_ERROR_SECTION,
								SPRIG_KRKR_TJS_W("DIB転送に失敗しました"),
								sprig::krkr::win32_error
								);
						}
					}
				}
				return 0;
			}
			return ::DefWindowProc(window_handle, message, w_param, l_param);
		}
	private:
		using WindowBase::window_handle_;
		using WindowBase::window_class_;
		using WindowBase::width_;
		using WindowBase::height_;
		using WindowBase::identity_;
		using WindowBase::visible_;
		using WindowBase::topMost_;
		using WindowBase::bitmap_info_;
		using WindowBase::bits_;
	private:
		void dispose() {
			instance_map_.erase(window_handle_);
			WindowBase::dispose();
		}
	public:
		Splash()
			: WindowBase()
		{
			window_class_.lpfnWndProc = static_cast<WNDPROC>(windowProcedure);
		}
		~Splash() throw() {
			dispose();
		}
		void initialize(
			size_type width,
			size_type height,
			string_type const& identity = string_type()
			)
		{
			RECT work_area_rect = { 0 };
			if (!::SystemParametersInfo(SPI_GETWORKAREA, 0, static_cast<PVOID>(&work_area_rect), 0)) {
				KTL_ERROR(
					KTL_ERROR_SECTION,
					SPRIG_KRKR_TJS_W("作業領域矩形の取得に失敗しました"),
					sprig::krkr::win32_error
					);
				return;
			}
			WindowBase::initialize(
				WS_EX_TOOLWINDOW,
				(work_area_rect.right - work_area_rect.left - width) / 2,
				(work_area_rect.bottom - work_area_rect.top - height) / 2,
				width,
				height,
				identity
				);
			instance_map_[window_handle_] = this;
		}
		bool loadImages(byte_type const* data, size_type size) {
			if (!WindowBase::loadImages(sprig::png::png_pixels_order_bgr, data, size)) {
				KTL_ERROR(
					KTL_ERROR_SECTION,
					SPRIG_KRKR_TJS_W("画像の読み込みに失敗しました"),
					sprig::krkr::win32_error
					);
				return false;
			}
			WindowUtils::updateWindow(window_handle_);
			return true;
		}
	};
}	// namespace ktl

#undef KTL_WARNING_SECTION
#undef KTL_ERROR_SECTION

#define KTL_WARNING_SECTION SPRIG_KRKR_TJS_W("KTL:LayeredSplashWindow:warning")
#define KTL_ERROR_SECTION SPRIG_KRKR_TJS_W("KTL:LayeredSplashWindow:error")

namespace ktl {
	//
	// LayeredSplash
	//
	class LayeredSplash
		: public WindowBase
	{
	public:
		typedef WindowBase::size_type size_type;
		typedef WindowBase::int_type int_type;
		typedef WindowBase::string_type string_type;
		typedef WindowBase::byte_type byte_type;
		typedef WindowBase::bits_type bits_type;
		typedef std::map<HWND, LayeredSplash*> instance_map_type;
	private:
		static instance_map_type instance_map_;
	private:
		static LRESULT CALLBACK windowProcedure(HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param) {
			return ::DefWindowProc(window_handle, message, w_param, l_param);
		}
	private:
		using WindowBase::window_handle_;
		using WindowBase::window_class_;
		using WindowBase::width_;
		using WindowBase::height_;
		using WindowBase::identity_;
		using WindowBase::visible_;
		using WindowBase::topMost_;
		using WindowBase::bitmap_info_;
		using WindowBase::bits_;
		byte_type alpha_;
		HBITMAP bitmap_handle_;
	private:
		void dispose() {
			if (bitmap_handle_) {
				if (!::DeleteObject(bitmap_handle_)) {
					KTL_ERROR_NOTHROW(
						KTL_ERROR_SECTION,
						SPRIG_KRKR_TJS_W("ビットマップの破棄に失敗しました")
						);
				}
				bitmap_handle_ = 0;
			}
			instance_map_.erase(window_handle_);
			WindowBase::dispose();
		}
	public:
		LayeredSplash()
			: WindowBase()
			, alpha_(255)
			, bitmap_handle_(0)
		{
			window_class_.lpfnWndProc = static_cast<WNDPROC>(windowProcedure);
			bitmap_info_.bmiHeader.biBitCount = 32;
		}
		~LayeredSplash() throw() {
			dispose();
		}
		void initialize(string_type const& identity = string_type()) {
			WindowBase::initialize(
				WS_EX_TOOLWINDOW | WS_EX_LAYERED,
				CW_USEDEFAULT,
				0,
				CW_USEDEFAULT,
				0,
				identity
				);
			instance_map_[window_handle_] = this;
		}
		bool loadImages(byte_type const* data, size_type size) {
			if (!window_handle_) {
				return false;
			}
			if (!WindowBase::loadImages(sprig::png::png_pixels_order_bgra, data, size)) {
				KTL_ERROR(
					KTL_ERROR_SECTION,
					SPRIG_KRKR_TJS_W("画像の読み込みに失敗しました"),
					sprig::krkr::win32_error
					);
				return false;
			}
			{
				boost::shared_ptr<boost::remove_pointer<HDC>::type> dc_handle(
					::GetDC(window_handle_),
					WindowUtils::release_dc(window_handle_)
					);
				if (!dc_handle) {
					KTL_ERROR(
						KTL_ERROR_SECTION,
						SPRIG_KRKR_TJS_W("デバイスコンテキストの取得に失敗しました"),
						sprig::krkr::win32_error
						);
				}
				if (bitmap_handle_) {
					if (!::DeleteObject(bitmap_handle_)) {
						KTL_ERROR(
							KTL_ERROR_SECTION,
							SPRIG_KRKR_TJS_W("ビットマップの破棄に失敗しました"),
							sprig::krkr::win32_error
							);
						return false;
					}
					bitmap_handle_ = 0;
				}
				VOID* pbits = 0;
				bitmap_handle_ = ::CreateDIBSection(
					boost::get_pointer(dc_handle),
					&bitmap_info_,
					DIB_RGB_COLORS,
					&pbits,
					0,
					0
					);
				if (!bitmap_handle_) {
					KTL_ERROR(
						KTL_ERROR_SECTION,
						SPRIG_KRKR_TJS_W("DIBセクションの作成に失敗しました"),
						sprig::krkr::win32_error
						);
					return false;
				}
				{
					byte_type* dst = static_cast<byte_type*>(pbits);
					byte_type const* src = &bits_[0];
					size_type pitch = bitmap_info_.bmiHeader.biWidth * 4;
					for (size_type y = 0, height = bitmap_info_.bmiHeader.biHeight; y < height; ++y) {
						for (size_type x = 0, width = bitmap_info_.bmiHeader.biWidth; x < width; ++x) {
							size_type pos = y * pitch + x * 4;
							byte_type a = src[pos + 3];
							dst[pos + 0] = src[pos + 0] * a / 255;
							dst[pos + 1] = src[pos + 1] * a / 255;
							dst[pos + 2] = src[pos + 2] * a / 255;
							dst[pos + 3] = a;
						}
					}
				}
			}
			WindowUtils::updateWindow(window_handle_);
			return true;
		}
		bool update(
			int_type x,
			int_type y,
			byte_type alpha
			)
		{
			if (!window_handle_) {
				return false;
			}
			if (bitmap_handle_) {
				if (!::SetWindowPos(
					window_handle_,
					0,
					x,
					y,
					bitmap_info_.bmiHeader.biWidth,
					bitmap_info_.bmiHeader.biHeight,
					SWP_NOMOVE | SWP_NOZORDER
					))
				{
					KTL_ERROR(
						KTL_ERROR_SECTION,
						SPRIG_KRKR_TJS_W("ウインドウのリサイズに失敗しました"),
						sprig::krkr::win32_error
						);
					return false;
				}
				width_ = bitmap_info_.bmiHeader.biWidth;
				height_ = bitmap_info_.bmiHeader.biHeight;

				boost::shared_ptr<boost::remove_pointer<HDC>::type> screen_dc_handle(
					::GetDC(0),
					WindowUtils::release_dc(0)
					);
				if (!screen_dc_handle) {
					KTL_ERROR(
						KTL_ERROR_SECTION,
						SPRIG_KRKR_TJS_W("デスクトップデバイスコンテキストの取得に失敗しました"),
						sprig::krkr::win32_error
						);
					return false;
				}
				boost::shared_ptr<boost::remove_pointer<HDC>::type> dc_handle(
					::GetDC(window_handle_),
					WindowUtils::release_dc(window_handle_)
					);
				if (!dc_handle) {
					KTL_ERROR(
						KTL_ERROR_SECTION,
						SPRIG_KRKR_TJS_W("デバイスコンテキストの取得に失敗しました"),
						sprig::krkr::win32_error
						);
					return false;
				}
				boost::shared_ptr<boost::remove_pointer<HDC>::type> compatible_dc_handle(
					::CreateCompatibleDC(boost::get_pointer(dc_handle)),
					WindowUtils::delete_dc()
					);
				if (!compatible_dc_handle) {
					KTL_ERROR(
						KTL_ERROR_SECTION,
						SPRIG_KRKR_TJS_W("互換デバイスコンテキストの作成に失敗しました"),
						sprig::krkr::win32_error
						);
					return false;
				}
				boost::shared_ptr<boost::remove_pointer<HGDIOBJ>::type> old_gdi_object_handle(
					::SelectObject(boost::get_pointer(compatible_dc_handle), bitmap_handle_),
					WindowUtils::select_object(boost::get_pointer(compatible_dc_handle))
					);
				if (!compatible_dc_handle) {
					KTL_ERROR(
						KTL_ERROR_SECTION,
						SPRIG_KRKR_TJS_W("ビットマップオブジェクトの選択に失敗しました"),
						sprig::krkr::win32_error
						);
					return false;
				}
				if (!::BitBlt(
					boost::get_pointer(dc_handle),
					0,
					0,
					bitmap_info_.bmiHeader.biWidth,
					bitmap_info_.bmiHeader.biHeight,
					boost::get_pointer(compatible_dc_handle),
					0,
					0,
					SRCCOPY | CAPTUREBLT
					))
				{
					KTL_ERROR(
						KTL_ERROR_SECTION,
						SPRIG_KRKR_TJS_W("ビットブロック転送に失敗しました"),
						sprig::krkr::win32_error
						);
					return false;
				}
				{
					POINT pos;
					pos.x = x;
					pos.y = y;
					SIZE size;
					size.cx = width_;
					size.cy = height_;
					POINT layer_pos;
					layer_pos.x = 0;
					layer_pos.y = 0;
					BLENDFUNCTION blend;
					blend.BlendOp = AC_SRC_OVER;
					blend.BlendFlags = 0;
					blend.SourceConstantAlpha = alpha;
					blend.AlphaFormat = AC_SRC_ALPHA;
					if (!::UpdateLayeredWindow(
						window_handle_,
						boost::get_pointer(screen_dc_handle),
						&pos,
						&size,
						boost::get_pointer(compatible_dc_handle),
						&layer_pos,
						0,
						&blend,
						ULW_ALPHA
						))
					{
						KTL_ERROR(
							KTL_ERROR_SECTION,
							SPRIG_KRKR_TJS_W("レイヤードウィンドウの更新に失敗しました"),
							sprig::krkr::win32_error
							);
						return false;
					}
				}
				x_ = x;
				y_ = y;
				alpha_ = alpha;
			}
			if (!::SetWindowPos(
				window_handle_,
				topMost_
					? HWND_TOPMOST
					: HWND_NOTOPMOST
					,
				x_,
				y_,
				width_,
				height_,
				SWP_NOMOVE | SWP_NOSIZE
				))
			{
				KTL_ERROR(
					KTL_ERROR_SECTION,
					SPRIG_KRKR_TJS_W("ウインドウのZオーダー変更に失敗しました"),
					sprig::krkr::win32_error
					);
				return false;
			}
			WindowUtils::updateWindow(window_handle_);
			return true;
		}
		bool updateToCenter(byte_type alpha) {
			RECT work_area_rect = { 0 };
			if (!::SystemParametersInfo(SPI_GETWORKAREA, 0, static_cast<PVOID>(&work_area_rect), 0)) {
				KTL_ERROR(
					KTL_ERROR_SECTION,
					SPRIG_KRKR_TJS_W("作業領域矩形の取得に失敗しました"),
					sprig::krkr::win32_error
					);
				return false;
			}
			return update(
				(work_area_rect.right - work_area_rect.left - bitmap_info_.bmiHeader.biWidth) / 2,
				(work_area_rect.bottom - work_area_rect.top - bitmap_info_.bmiHeader.biHeight) / 2,
				alpha
				);
		}
		byte_type getOpacity() const {
			return alpha_;
		}
	};
}	// namespace ktl

#undef KTL_WARNING_SECTION
#undef KTL_ERROR_SECTION
