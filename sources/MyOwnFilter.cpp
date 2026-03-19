#include "MyOwnFilter.h"
#include "UserFilter.h"
#include "AVL_Lite.h"

#include "UserFilterLibrary.hxx"

namespace avs
{

	// Example image processing filter
	class CustomThreshold : public UserFilter
	{
	private:
		avl::Image outImage;

	public:
		/// Defines the inputs, the outputs and the metadata
		void Define() override
		{
			SetName		(L"CustomThreshold");
			SetCategory	(L"Image::Image Thresholding");
			SetImage	(L"CustomThreshold_16.png");	// Images are stored in "bin" directory where DLL is saved.
			SetImageBig	(L"CustomThreshold_48.png");
			SetTip		(L"Binarizes 8-bit images");
			//					 Name						Type				Default		Tool-tip
			AddInput			(L"inImage",				L"Image",			L"",		L"Input image"    );
			AddInput			(L"inThreshold",			L"Integer<0, 255>",	L"128",		L"Threshold value");
			AddOutput			(L"outImage",				L"Image",						L"Output image"   );
			AddDiagnosticOutput	(L"diagFillingPercentage",	L"Real",						L"Filling of image in percents"	);

			// This attribute is relevant only when preparing this user filter for code generation. It describes a function that should be used as an equivalent of this filter.
			AddAttribute(L"CodeGenInfo", L"CustomThreshold( in: inImage; in: inThreshold; out: outImage; out: diagFillingPercentage ) Comment(\"This function must be defined separately to replace a user filter.\")");
		}

		/// Computes output from input data
		int Invoke() override
		{
			// Get data from the inputs
			avl::Image inImage;
			int inThreshold;
			ReadInput(L"inImage", inImage);
			ReadInput(L"inThreshold", inThreshold);

			// Test pixel format
			if (inImage.Type() != avl::PlainType::UInt8)
			{
				// This message will be shown in the console of AVS
				throw atl::DomainError(L"Only uint8 pixel format supported.");
			}

			// Get image properties
			int  height = inImage.Height();
			int  width  = inImage.Width();
			int  depth  = inImage.Depth();

			// Prepare output image
			outImage.Reset(inImage, atl::NIL);

			// For each row
			for (int y = 0; y < height; ++y)
			{
				// Get row pointers
				const atl::uint8* p = inImage.RowBegin<atl::uint8>(y);
				const atl::uint8* e = inImage.RowEnd<atl::uint8>(y);
				atl::uint8*       q = outImage.RowBegin<atl::uint8>(y);
				// Loop over the pixel components
				while (p < e)
				{
					(*q++) = (*p++) < inThreshold ? 0 : 255;
				}

				// Check if user pressed the Stop button.
				ThrowIfCancelled();
			}

			// Count filling percentage for diagnostic execution mode
			if (IsDiagnosticMode())
			{
				long filled = 0;
				long all = width * height * depth;
				for (int y = 0; y < height; ++y)
				{
					const atl::uint8* q = outImage.RowBegin<atl::uint8>(y);
					const atl::uint8* e = outImage.RowEnd<atl::uint8>(y);
					while (q < e)
					{
						if ((*q++) == 0)
							filled++;
					}

					// Check if user pressed the Stop button.
					ThrowIfCancelled();
				}
				float fillingPercentage = all > 0 ? ((float)filled * 100 / all) : 0;
				WriteOutput(L"diagFillingPercentage", fillingPercentage);
			}

			// Set output data
			WriteOutput(L"outImage", outImage);

			// Continue program
			return INVOKE_NORMAL;
		}
	};


	/// Builds the filter factory
	class RegisterUserObjects
	{
	public:
		RegisterUserObjects()
		{
			// Remember to register every filter exported by the user filter library
			RegisterFilter(CreateInstance<CustomThreshold>);
		}
	};

	static RegisterUserObjects registerUserObjects;

}
