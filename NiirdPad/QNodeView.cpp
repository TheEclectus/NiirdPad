#include "QNodeView.h"

#include <QMenu>

#include "GraphicsBlocks.h"
#include "Node.h"

void QNodeView::Input()
{
	// TODO: Consider pros and cons of flushing event cache, sans user events.

	SDL_Event Event;
	while (SDL_PollEvent(&Event))
	{
		if (Event.type == EVENT_MOUSEDOWN)
		{
			SDL_Point NewDownPos = { reinterpret_cast<int>(Event.user.data1), reinterpret_cast<int>(Event.user.data2) };

			if (Event.user.code == Qt::MouseButton::LeftButton)
				_InputState.DownPosition[0] = NewDownPos;
			else if (Event.user.code == Qt::MouseButton::RightButton)
				_InputState.DownPosition[1] = NewDownPos;
			else if (Event.user.code == Qt::MouseButton::MiddleButton)
				_InputState.DownPosition[2] = NewDownPos;
			//printf("MouseDown (%d, %d)\n", reinterpret_cast<int>(Event.user.data1), reinterpret_cast<int>(Event.user.data2));

			/*
				===================
				NODE CLICK SCANNING
				===================
			*/
			auto KeyboardMods = SDL_GetModState();
			bool bNodeFound = false;
			// TODO: Only look through the list of VISIBILE nodes (see TODO about occlusion culling)
			for (auto CurNode : _Nodes)
			{
				auto NodePos = CurNode->Position();
				auto NodeBounds = CurNode->Graphics().GetBounds();
				NodeBounds.x = (_Camera.ViewBox.w / 2) - _Camera.ViewBox.x + NodePos.x;
				NodeBounds.y = (_Camera.ViewBox.h / 2) - _Camera.ViewBox.y + NodePos.y;

				// A Node is found
				if (SDL_PointInRect(&NewDownPos, &NodeBounds))
				{
					// Left clicks
					if (Event.user.code == Qt::MouseButton::LeftButton)
					{
						auto &SelNodes = _InputState.SelectedNodes;
						if (!(KeyboardMods & KMOD_CTRL))	// Neither Ctrl pressed
						{
							SelNodes.clear();
						}

						auto FindRes = std::find(SelNodes.begin(), SelNodes.end(), CurNode);
						// If it's not already selected
						if (FindRes == SelNodes.end())
						{
							SelNodes.push_back(CurNode);
						}

						bNodeFound = true;
						break;
					}
				}
			}

			/*
			NEXTTIME: 
			- See TODOs about occlusion cullings; 
			- SDL_GetModState() doesn't appear to be working, see about inheriting from QMainWindow::keyPressEvent and ::keyReleaseEvent and piping it to the QSDLPanel (from NiirdPad.cpp);
			- Finish implementing methods for Node and related classes
			*/

			// No node found, it was a left-click and Ctrl isn't down
			if (!bNodeFound && (Event.user.code == Qt::MouseButton::LeftButton) && !(KeyboardMods & KMOD_LCTRL))
			{
				_InputState.SelectedNodes.clear();
			}
		}
		else if (Event.type == EVENT_MOUSEUP)
		{
			if (Event.user.code == Qt::MouseButton::LeftButton)
				_InputState.DownPosition[0] = { -1, -1 };
			else if (Event.user.code == Qt::MouseButton::RightButton)
				_InputState.DownPosition[1] = { -1, -1 };
			else if (Event.user.code == Qt::MouseButton::MiddleButton)
				_InputState.DownPosition[2] = { -1, -1 };
			//printf("MouseUp (%d, %d)\n", reinterpret_cast<int>(Event.user.data1), reinterpret_cast<int>(Event.user.data2));

			SDL_Point ReleasePos = { reinterpret_cast<int>(Event.user.data1), reinterpret_cast<int>(Event.user.data2) };

			//printf("MouseDown (%d, %d)\n", reinterpret_cast<int>(Event.user.data1), reinterpret_cast<int>(Event.user.data2));

			/*
				===================
				NODE CLICK SCANNING
				===================
			*/
			auto KeyboardMods = SDL_GetModState();
			bool bNodeFound = false;
			// TODO: Only look through the list of VISIBILE nodes (see TODO about occlusion culling)
			for (auto CurNode : _Nodes)
			{
				auto NodePos = CurNode->Position();
				auto NodeBounds = CurNode->Graphics().GetBounds();
				NodeBounds.x = (_Camera.ViewBox.w / 2) - _Camera.ViewBox.x + NodePos.x;
				NodeBounds.y = (_Camera.ViewBox.h / 2) - _Camera.ViewBox.y + NodePos.y;

				// A Node is found
				if (SDL_PointInRect(&ReleasePos, &NodeBounds))
				{
					if (Event.user.code == Qt::MouseButton::RightButton)
					{
						SDL_Point PointInsideNode = { ReleasePos.x - NodeBounds.x, ReleasePos.y - NodeBounds.y };

						NodeDialogue *Dlg = nullptr;
						NodeOption *Opt = nullptr;
						if (CurNode->FeatureAtPosition(PointInsideNode, &Dlg, &Opt))
						{
							// TODO: scrolling lags if immediately done while menu is open
							QMenu Context("Context Menu", this);
							if (Dlg)
							{
								Context.addAction("Edit Dialogue");
								Context.addAction("Delete Dialogue");

								Context.exec(mapToGlobal(QPoint(ReleasePos.x, ReleasePos.y)));
							}
						}

						break;
					}
				}
			}
		}
		else if (Event.type == EVENT_MOUSEMOVE)
		{
			_InputState.Position = GetMousePosition();// { reinterpret_cast<int>(Event.user.data1), reinterpret_cast<int>(Event.user.data2) };
			//printf("MouseMove [%c%c%c] (%d, %d)\n", Event.user.code & Qt::MouseButton::LeftButton ? 'L' : ' ', Event.user.code & Qt::MouseButton::MiddleButton ? 'M' : ' ', Event.user.code & Qt::MouseButton::RightButton ? 'R' : ' ', reinterpret_cast<int>(Event.user.data1), reinterpret_cast<int>(Event.user.data2));
			if (Event.user.code & Qt::MouseButton::MiddleButton)
			{
				SDL_Point Delta = { reinterpret_cast<int>(Event.user.data1), reinterpret_cast<int>(Event.user.data2) };
				//printf_s("%d %d\n", Delta.x, Delta.y);
				_Camera.ViewBox.x -= Delta.x;
				_Camera.ViewBox.y -= Delta.y;
			}
		}

		else if (Event.type == SDL_EventType::SDL_WINDOWEVENT)
		{
			SDL_WindowEvent &WinEvent = Event.window;
			if (WinEvent.event == SDL_WindowEventID::SDL_WINDOWEVENT_RESIZED)
			{
				int NewWidth = WinEvent.data1;
				int NewHeight = WinEvent.data2;

				_Camera.ViewBox.w = NewWidth;
				_Camera.ViewBox.h = NewHeight;

				printf_s("Camera adjusted to %dx%d.\n", _Camera.ViewBox.w, _Camera.ViewBox.h);
			}
		}

		else if (Event.type == SDL_EventType::SDL_RENDER_TARGETS_RESET)
		{
			printf_s("SuS\n");
			_FontStore.ResetFonts((SDL_EventType)Event.type);
		}
		else if (Event.type == SDL_EventType::SDL_RENDER_DEVICE_RESET)
		{
			printf_s("sUs\n");
			_FontStore.ResetFonts((SDL_EventType)Event.type);
		}
	}
}

void QNodeView::Logic()
{
	
}

void QNodeView::Render()
{
	RenderBackground();

	RenderForeground();
}

void QNodeView::mousePressEvent(QMouseEvent *event)
{
	QSDLPanel::mousePressEvent(event);

	//event->
	// NEXTTIME: Context menu for Nodes!
}

void QNodeView::mouseReleaseEvent(QMouseEvent *event)
{
	QSDLPanel::mouseReleaseEvent(event);
}

void QNodeView::mouseMoveEvent(QMouseEvent *event)
{
	QSDLPanel::mouseMoveEvent(event);
}

void QNodeView::RenderBackground()
{
	const int GridSize = 75;

	SDL_Renderer *Renderer = SDLRenderer();

	SDL_Point TopLeft = { _Camera.ViewBox.x - (_Camera.ViewBox.w / 2), _Camera.ViewBox.y - (_Camera.ViewBox.h / 2) };
	int XStartOffset = TopLeft.x % GridSize;
	int YStartOffset = TopLeft.y % GridSize;

	SDL_SetRenderDrawColor(Renderer, _GridLineColor.r, _GridLineColor.g, _GridLineColor.b, _GridLineColor.a);
	// TODO: Draw lines in batches with SDL_RenderDrawLines().
	for (int i = 0; i <= _Camera.ViewBox.w / GridSize; i++)
	{
		int XPosition = (i * GridSize) - XStartOffset;
		SDL_RenderDrawLine(Renderer, XPosition, 0, XPosition, _Camera.ViewBox.h);
	}

	for (int i = 0; i <= _Camera.ViewBox.h / GridSize; i++)
	{
		int YPosition = (i * GridSize) - YStartOffset;
		SDL_RenderDrawLine(Renderer, 0, YPosition, _Camera.ViewBox.w, YPosition);
	}
}

#include "GraphicsBlocks.h"

void QNodeView::RenderForeground()
{
	SDL_Point TopLeft = { _Camera.ViewBox.x - (_Camera.ViewBox.w / 2), _Camera.ViewBox.y - (_Camera.ViewBox.h / 2) };

	SDL_Renderer *Renderer = SDLRenderer();

	// TODO: occlusion culling (list of visible nodes, updated on camera movement)
	for (auto Node : _Nodes)
	{
		SDL_Point NodePos = Node->Position();
		SDL_Point BlockRenderPos = { 0, 0 };
		BlockRenderPos.x = (_Camera.ViewBox.w / 2) - _Camera.ViewBox.x + NodePos.x;
		BlockRenderPos.y = (_Camera.ViewBox.h / 2) - _Camera.ViewBox.y + NodePos.y;

		// TODO: Only check the visibile
		if (std::find(_InputState.SelectedNodes.begin(), _InputState.SelectedNodes.end(), Node) != _InputState.SelectedNodes.end())
		{
			SDL_Point NodePos = Node->Position();
			SDL_Rect HighlightRect = Node->Graphics().GetBounds();
			HighlightRect.x = ((_Camera.ViewBox.w / 2) - _Camera.ViewBox.x) - 2 + NodePos.x;
			HighlightRect.y = ((_Camera.ViewBox.h / 2) - _Camera.ViewBox.y) - 2 + NodePos.y;
			HighlightRect.w += 4;
			HighlightRect.h += 4;

			auto Renderer = SDLRenderer();
			SDL_SetRenderDrawColor(Renderer, 255, 255, 255, 255);
			SDL_RenderDrawRect(Renderer, &HighlightRect);
		}

		Node->Graphics().Render(Renderer, BlockRenderPos);
	}
}

QNodeView::QNodeView(QWidget *Parent) :
	QSDLPanel(Parent),
	_FontStore(this->SDLRenderer())
{
	auto NewNode = new Node(*this);
	auto NN_Dlg = NewNode->AddDialogue("blep");
	NN_Dlg->SetDialogue("Test!");
	NN_Dlg->SetFunctions({ "give_money \"krats\" 20" });

	//auto NN_Opt = NewNode->AddOption();
	//NN_Opt->SetAll({ "//showif.has_krats.20", "//hideif.has_krats.50", "//hideif.has_adats.50" }, { "take_money krats 20" }, "BEEEEEEEEEP ( ' v ')");

	_Nodes.push_back(NewNode);
}

const FontStore &QNodeView::FontStore() const
{
	return _FontStore;
}