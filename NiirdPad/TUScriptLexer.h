#pragma once

#include <set>
#include <string>
#include <vector>

#include <tao\pegtl.hpp>

namespace pegtl = tao::pegtl;

namespace TUScript
{
	struct State
	{
		struct Fragment
		{
			struct Dialogue
			{
				std::string Reference = "";
				std::vector<std::string> Functions = {};
				std::string Text = "";

				bool operator==(const Dialogue &RHS) const
				{
					return (
						Reference == RHS.Reference &&
						Functions == RHS.Functions &&
						Text == RHS.Text
					);
				}
				bool operator!=(const Dialogue &RHS) const
				{
					return !operator==(RHS);
				}
			} PendingDialogue;

			struct Option
			{
				std::string Pointer = "";
				std::vector<std::string> VisibilityScripts = {}, Functions = {};
				std::string Text = "";

				bool operator==(const Option &RHS) const
				{
					return (
						Pointer == RHS.Pointer &&
						VisibilityScripts == RHS.VisibilityScripts &&
						Functions == RHS.Functions &&
						Text == RHS.Text
						);
				}
				bool operator!=(const Option &RHS) const
				{
					return !operator==(RHS);
				}
			} PendingOption;

			std::string Comment = "";
			std::vector<Dialogue> Dialogues;
			std::vector<Option> Options;

			void PushPendingDialogue()
			{
				Dialogues.push_back(PendingDialogue);
				PendingDialogue = Dialogue{};
			}
			void PushPendingOption()
			{
				Options.push_back(PendingOption);
				PendingOption = Option{};
			}
			bool operator==(const Fragment &RHS) const
			{
				return (
					Comment == RHS.Comment &&
					Dialogues == RHS.Dialogues
				);
			}
			bool operator!=(const Fragment &RHS) const
			{
				return !operator==(RHS);
			}
		} PendingFragment;

		std::string GlobalComment;
		std::vector<Fragment> Fragments;

		std::vector<std::string> Warnings;
		std::string Error;

		void PushPendingFragment()
		{
			// Make sure it's not empty.
			if (PendingFragment != State::Fragment{})
			{
				Fragments.push_back(PendingFragment);
				PendingFragment = State::Fragment{};
			}
		}
	};

	struct Whitespace : pegtl::sor<pegtl::one<' '>, pegtl::one<'\t'>> {};
	struct AnyWhitespace : pegtl::star<Whitespace> {};
	template<class T>
	struct WrapWhitespace : pegtl::seq<AnyWhitespace, T, AnyWhitespace> {};
	struct RestOfLine : pegtl::until<pegtl::at<pegtl::one<'\n'>>> {};
	struct ReferenceChar : pegtl::sor<pegtl::alnum, pegtl::one<'_'>> {};
	struct ScriptLineChar : pegtl::sor< pegtl::alnum, pegtl::one<'_'>, pegtl::one<' '>, pegtl::one<'/'> > {};
	struct TextChar : pegtl::not_one<'/', '\n'> {};
	struct VisScriptChar : pegtl::sor<pegtl::alnum, pegtl::one<'_'>, pegtl::one<'.'>> {};

	struct CommentPrefix : TAO_PEGTL_STRING(">>") {};
	struct CommentGlobalContents : RestOfLine {};
	struct CommentGlobal : pegtl::seq<CommentPrefix, CommentGlobalContents> {};

	struct SeparatorPrefix : TAO_PEGTL_STRING("||") {};
	struct CommentFragmentContents : RestOfLine {};
	// Fragment comment (follows separator)
	struct CommentFragment : pegtl::seq<CommentPrefix, CommentFragmentContents> {};
	struct Separator : pegtl::seq<SeparatorPrefix, pegtl::star<CommentFragment>> {};

	struct DialogueReferenceDeclaration : pegtl::plus<ReferenceChar> {};
	struct DialogueChunkHeaderScriptsContent : pegtl::plus<ScriptLineChar> {};
	struct DialogueChunkHeaderScriptsContentList : pegtl::list<DialogueChunkHeaderScriptsContent, pegtl::one<','>, pegtl::one<' '>> {};
	struct DialogueChunkHeaderScripts : pegtl::seq< pegtl::pad<pegtl::one<'|'>, pegtl::one<' '>>, DialogueChunkHeaderScriptsContentList > {};
	struct DialogueChunkHeader : pegtl::seq< pegtl::one<'{'>, DialogueReferenceDeclaration, pegtl::opt<DialogueChunkHeaderScripts>, pegtl::one<'}'> > {};
	struct DialogueChunkContent : RestOfLine {};
	struct DialogueChunk : pegtl::seq< DialogueChunkHeader, WrapWhitespace<DialogueChunkContent> > {};



	struct Grammar : pegtl::plus< pegtl::sor<
		CommentGlobal,
		Separator,
		DialogueChunk,
		OptionChunk,
		pegtl::eolf
	>> {};

	template< typename Rule >
	struct Action
	{};

	template<>
	struct Action< CommentGlobalContents >
	{
		template< typename Input >
		static void apply(const Input& in, State& v)
		{
			v.GlobalComment = in.string();
		}
	};

	template<>
	struct Action< CommentFragmentContents >
	{
		template< typename Input >
		static void apply(const Input& in, State& v)
		{
			v.PendingFragment.Comment = in.string();
		}
	};

	template<>
	struct Action< SeparatorPrefix >
	{
		template< typename Input >
		static void apply(const Input& in, State& v)
		{
			v.PushPendingFragment();
		}
	};

	template<>
	struct Action< DialogueReferenceDeclaration >
	{
		template< typename Input >
		static void apply(const Input& in, State& v)
		{
			v.PendingFragment.PendingDialogue.Reference = in.string();
		}
	};

	template<>
	struct Action< DialogueChunkHeaderScriptsContent >
	{
		template< typename Input >
		static void apply(const Input& in, State& v)
		{
			v.PendingFragment.PendingDialogue.Functions.push_back(in.string());
		}
	};

	template<>
	struct Action< DialogueChunkContent >
	{
		template< typename Input >
		static void apply(const Input& in, State& v)
		{
			v.PendingFragment.PendingDialogue.Text = in.string();
		}
	};

	template<>
	struct Action< DialogueChunk >
	{
		template< typename Input >
		static void apply(const Input& in, State& v)
		{
			v.PendingFragment.PushPendingDialogue();
		}
	};

	template<>
	struct Action< OptionChunkHeaderPointerDeclaration >
	{
		template< typename Input >
		static void apply(const Input& in, State& v)
		{
			v.PendingFragment.PendingOption.Pointer = in.string();
		}
	};

	template<>
	struct Action< OptionChunkHeaderReferenceNoFunctions >
	{
		template< typename Input >
		static void apply(const Input& in, State& v)
		{
			printf_s("OptionChunkHeaderReferenceNoFunctions\n");
		}
	};

	template<>
	struct Action< OptionChunkHeaderScriptsContent >
	{
		template< typename Input >
		static void apply(const Input& in, State& v)
		{
			v.PendingFragment.PendingOption.Functions.push_back(in.string());
		}
	};

	template<>
	struct Action< OptionChunkHeaderNoReferenceFunctions >
	{
		template< typename Input >
		static void apply(const Input& in, State& v)
		{
			printf_s("OptionChunkHeaderNoReferenceFunctions\n");
		}
	};

	template<>
	struct Action< OptionChunkContent >
	{
		template< typename Input >
		static void apply(const Input& in, State& v)
		{
			v.PendingFragment.PendingOption.Text = in.string();
		}
	};

	template<>
	struct Action< OptionChunkVisScriptContent >
	{
		template< typename Input >
		static void apply(const Input& in, State& v)
		{
			v.PendingFragment.PendingOption.VisibilityScripts.push_back(in.string());
		}
	};

	template<>
	struct Action< OptionChunk >
	{
		template< typename Input >
		static void apply(const Input& in, State& v)
		{
			v.PendingFragment.PushPendingOption();
		}
	};

	template<>
	struct Action< pegtl::eof >
	{
		template< typename Input >
		static void apply(const Input& in, State& v)
		{
			v.PushPendingFragment();
		}
	};
}