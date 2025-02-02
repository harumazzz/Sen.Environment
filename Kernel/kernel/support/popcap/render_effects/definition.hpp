#pragma once

#include "kernel/utility/utility.hpp"

namespace Sen::Kernel::Support::PopCap::RenderEffects {

	#pragma region Block1

	struct Block1 {
		public:
			uint32_t unknown_1;
			uint32_t unknown_2;
			uint32_t unknown_3;
			uint32_t unknown_4;
			uint32_t unknown_5;
			uint32_t unknown_6;

			explicit Block1(

			) = default;

			explicit Block1(
				uint32_t unknown_1,
				uint32_t unknown_2,
				uint32_t unknown_3,
				uint32_t unknown_4,
				uint32_t unknown_5,
				uint32_t unknown_6
			) : unknown_1(unknown_1), unknown_2(unknown_2), unknown_3(unknown_3),
			unknown_4(unknown_4), unknown_5(unknown_5), unknown_6(unknown_6)
			{

			}

			~Block1(

			) = default;
	};

	#pragma endregion

	#pragma region Block2

	struct Block2 {
		public:
			uint32_t unknown_1;
			uint32_t unknown_2;

			explicit Block2(

			) = default;

			explicit Block2(
				uint32_t unknown_1,
				uint32_t unknown_2
			) :  unknown_1(unknown_1), unknown_2(unknown_2)
			{

			}

			~Block2(

			) = default;
	};

	#pragma endregion

	#pragma region Block3

	struct Block3 {
		public:
			uint32_t unknown_2;
			std::string string;

			explicit Block3(
				uint32_t unknown_2,
				const std::string & string
			) : unknown_2(unknown_2), string(string)
			{ 

			}

			explicit Block3(

			) = default;

			~Block3(

			) = default;
	};

	#pragma endregion

	#pragma region Block4

	struct Block4 {
		public:
			uint32_t unknown_1;
			uint32_t unknown_2;
			uint32_t unknown_3;
			uint32_t unknown_4;
			uint32_t unknown_5;

			explicit Block4(

			) = default;

			explicit Block4(
				uint32_t unknown_1,
				uint32_t unknown_2,
				uint32_t unknown_3,
				uint32_t unknown_4,
				uint32_t unknown_5
			) : unknown_1(unknown_1), unknown_2(unknown_2), unknown_3(unknown_3),
			unknown_4(unknown_4), unknown_5(unknown_5)
			{

			}

			~Block4(

			) = default;
	};

	#pragma endregion

	#pragma region Block5

	struct Block5 {
		public:
			uint32_t unknown_1;
			uint32_t unknown_2;
			uint32_t unknown_3;
			uint32_t unknown_4;
			uint32_t unknown_5;
			uint32_t unknown_6;
			uint32_t unknown_7;

			explicit Block5(

			) = default;

			explicit Block5(
				uint32_t unknown_1,
				uint32_t unknown_2,
				uint32_t unknown_3,
				uint32_t unknown_4,
				uint32_t unknown_5,
				uint32_t unknown_6,
				uint32_t unknown_7
			) : unknown_1(unknown_1), unknown_2(unknown_2), unknown_3(unknown_3),
			unknown_4(unknown_4), unknown_5(unknown_5), unknown_6(unknown_6), unknown_7(unknown_7)
			{

			}

			~Block5(

			) = default;
	};
		

	#pragma endregion

	#pragma region Block6

	struct Block6 {
		public:
			uint32_t unknown_1;
			uint32_t unknown_2;
			uint32_t unknown_3;
			uint32_t unknown_4;
			uint32_t unknown_5;

			explicit Block6(

			) = default;

			explicit Block6(
				uint32_t unknown_1,
				uint32_t unknown_2,
				uint32_t unknown_3,
				uint32_t unknown_4,
				uint32_t unknown_5
			) : unknown_1(unknown_1), unknown_2(unknown_2), unknown_3(unknown_3),
			unknown_4(unknown_4), unknown_5(unknown_5)
			{

			}

			~Block6(

			) = default;
	};

	#pragma endregion

	#pragma region Block7

	struct Block7 {
		public:
			uint32_t unknown_1;
			uint32_t unknown_2;

			explicit Block7(

			) = default;

			explicit Block7(
				uint32_t unknown_1,
				uint32_t unknown_2
			) : unknown_1(unknown_1), unknown_2(unknown_2)
			{

			}

			~Block7(

			) = default;
	};

	#pragma endregion

	#pragma region Block8

	struct Block8 {
		public:
			uint32_t unknown_1;
			uint32_t unknown_2;
			uint32_t unknown_3;
			uint32_t unknown_4;
			uint32_t unknown_5;

			explicit Block8(

			) = default;

			explicit Block8(
				uint32_t unknown_1,
				uint32_t unknown_2,
				uint32_t unknown_4,
				uint32_t unknown_5,
				uint32_t unknown_3
			) : unknown_1(unknown_1), unknown_2(unknown_2), unknown_3(unknown_3),
			unknown_4(unknown_4), unknown_5(unknown_5)
			{

			}

			~Block8(

			) = default;
	};

	#pragma endregion

	#pragma region RenderEffects 	

	struct PopCapRenderEffects
	{
		public:
			List<Block1> block_1;
			List<Block2> block_2;
			List<Block3> block_3;
			List<Block4> block_4;
			List<Block5> block_5;
			List<Block6> block_6;
			List<Block7> block_7;
			List<Block8> block_8;

			explicit PopCapRenderEffects(

			) = default;

			explicit PopCapRenderEffects(
				List<Block1> block_1,
				List<Block2> block_2,
				List<Block3> block_3,
				List<Block4> block_4,
				List<Block5> block_5,
				List<Block6> block_6,
				List<Block7> block_7,
				List<Block8> block_8
			)
			{
				thiz.block_1 = std::move(block_1);
				thiz.block_2 = std::move(block_2);
				thiz.block_3 = std::move(block_3);
				thiz.block_4 = std::move(block_4);
				thiz.block_5 = std::move(block_5);
				thiz.block_6 = std::move(block_6);
				thiz.block_7 = std::move(block_7);
				thiz.block_8 = std::move(block_8);
			}

			~PopCapRenderEffects(

			) = default;
	};

	#pragma endregion
}