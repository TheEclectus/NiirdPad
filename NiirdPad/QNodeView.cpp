#include "QNodeView.h"

#include <QFile>

#include "DialogueFile.h"
#include "GraphicsBlocks.h"
#include "NiirdPad.h"
#include "Node.h"
#include "QReferenceEditWindow.h"
#include "QScriptEditWindow.h"

void QNodeView::Input()
{
	// TODO: Consider pros and cons of flushing event cache, sans user events.

	SDL_Event Event;
	while (SDL_PollEvent(&Event))
	{
		#pragma region EVENT_MOUSEDOWN
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

			if (_InputState.MousedOverNub != nullptr && Event.user.code == Qt::MouseButton::LeftButton)
			{
				_InputState.DraggingNub = _InputState.MousedOverNub;
				return;
			}

			/*
				===================
				NODE CLICK SCANNING
				===================
			*/
			auto KeyboardMods = SDL_GetModState();
			bool bNodeFound = false;
			// TODO: Only look through the list of VISIBILE nodes (see TODO about occlusion culling)
			for (auto CurNodeIter = _Nodes.begin(); CurNodeIter != _Nodes.end(); CurNodeIter++)
			{
				auto CurNode = *CurNodeIter;

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
						
						// Brings last-clicked Node to the top
						_Nodes.erase(CurNodeIter);
						_Nodes.insert(_Nodes.begin(), CurNode);

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
		#pragma endregion
		#pragma region EVENT_MOUSEUP
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

			// It's only going to be set if the nub currently has the mouse over it, so.
			if (_InputState.MousedOverNub != nullptr)
			{
				if (Event.user.code == Qt::MouseButton::RightButton)
				{
					auto Nub = _InputState.MousedOverNub;
					_InputState.MousedOverNub = nullptr;
					QMenu Context("Context Menu", this);

					auto M = Context.addMenu("Nexivian");
					auto M2 = M->addMenu("Bank Account");
					M->addMenu("FurAffinity Account");
					auto M3 = M->addMenu("Reputation");

					M2->addAction("Edit");
					M2->addAction("Delete");

					M3->addAction("Silly Doodles");
					M3->addAction("Commissions");
					M3->addAction("Secret Gore Alt");
					auto M4 = M3->addMenu("Hunt for the Bog Wife");

					M4->addAction("\"Give me a hint\"");
					M4->addAction("<<< HER LOCATION >>>")->setEnabled(false);

					Context.exec(mapToGlobal(QPoint(ReleasePos.x, ReleasePos.y)));
				}

				if (Event.user.code == Qt::MouseButton::LeftButton && _InputState.DraggingNub != nullptr)
				{
					// Connection code
				}
				//return;
			}

			if (Event.user.code == Qt::MouseButton::LeftButton)
			{
				_InputState.bDraggingNodes = false;
				_InputState.DraggingNub = nullptr;
				return;
			}

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

						bool bHeader = false;
						bool bDlgSection = false;
						bool bOptSection = false;
						NodeDialogue *Dlg = nullptr;
						NodeOption *Opt = nullptr;

						CurNode->FeatureAtPosition(PointInsideNode, bHeader, bDlgSection, bOptSection, &Dlg, &Opt);
						if (bHeader || bDlgSection || bOptSection)
						{
							// TODO: scrolling lags if immediately done while menu is open (consider tying refresh rate to Qt's Dirty()ing)
							QMenu Context("Context Menu", this);
							if (bHeader)
							{
								Context.addAction("Edit Comment", [this, CurNode]() {
									
								});
							}

							if (bDlgSection)
							{
								Context.addAction("New Dialogue");
								if (Dlg)
								{
									// NEXTTIME: See if you can't pass the NiirdPad instance as the parent, somehow. The refreshing stops and only the background color is shown when the edit window is opened.
									Context.addAction("Edit Dialogue", [this, Dlg]() {
										//QScriptEditWindow::EditDialogueFragment(_Parent->window(), *this->_Engine, *Dlg);
										this->_Parent->ScriptEditWindow()->dialogueFragment(Dlg);
									});
									Context.addAction("Delete Dialogue");
									Context.addSeparator();
									Context.addAction("Edit Index", [this, Dlg] {
										//std::string Reference = Dlg->GetReference();
										this->_Parent->ReferenceEditWindow()->editReference(Dlg);
									});
									Context.addSeparator();
								}
								else
								{
									Context.addSeparator();
								}
							}

							Context.addAction("Delete Node");

							Context.exec(mapToGlobal(QPoint(ReleasePos.x, ReleasePos.y)));
							return;
						}

						break;
					}
				}
			}
			// No Node is found
			
			if (Event.user.code == Qt::MouseButton::RightButton)
			{
				QMenu Context("Context Menu", this);
				Context.addAction("Create Node");
				Context.exec(mapToGlobal(QPoint(ReleasePos.x, ReleasePos.y)));
				return;
			}
		}
		#pragma endregion
		#pragma region EVENT_MOUSEMOVE
		else if (Event.type == EVENT_MOUSEMOVE)
		{
			SDL_Point MousePos = GetMousePosition();
			_InputState.Position = MousePos;// { reinterpret_cast<int>(Event.user.data1), reinterpret_cast<int>(Event.user.data2) };
			//printf("MouseMove [%c%c%c] (%d, %d)\n", Event.user.code & Qt::MouseButton::LeftButton ? 'L' : ' ', Event.user.code & Qt::MouseButton::MiddleButton ? 'M' : ' ', Event.user.code & Qt::MouseButton::RightButton ? 'R' : ' ', reinterpret_cast<int>(Event.user.data1), reinterpret_cast<int>(Event.user.data2));
			
			// Quick cycle-saving trick: if MousedOverNub != nullptr, check it against the mouse position first.
			if (_InputState.MousedOverNub != nullptr)
			{
				SDL_Point CurNubPos = { 0, 0 };
				SDL_Rect NodeBounds = { 0, 0, 0, 0 };
				SDL_Point NodePos = { 0, 0 };

				if (_InputState.MousedOverNub->GetNubType() == ANub::NubType::Output)
				{
					NodeOption &CurOpt = static_cast<NubOutput*>(_InputState.MousedOverNub)->Parent();
					CurNubPos = CurOpt.Graphics()->NubPoint();
					NodeBounds = CurOpt.Parent().Graphics().GetBounds();
					NodePos = CurOpt.Parent().Position();
				}
				else if (_InputState.MousedOverNub->GetNubType() == ANub::NubType::Input)
				{

				}
				

				NodeBounds.x = (_Camera.ViewBox.w / 2) - _Camera.ViewBox.x + NodePos.x;
				NodeBounds.y = (_Camera.ViewBox.h / 2) - _Camera.ViewBox.y + NodePos.y;

				CurNubPos.x += NodeBounds.x;
				CurNubPos.y += NodeBounds.y;

				// Pythagorean theorem to get the distance from the center of the nub.
				SDL_Point Distances = { CurNubPos.x - MousePos.x, CurNubPos.y - MousePos.y };
				int DistanceFromCenter = sqrt(pow(Distances.x, 2) + pow(Distances.y, 2));

				// Radius = (W - 1) / 2
				const int NubRadius = (NubOutput::TextureSize().w - 1) / 2;

				// Opposite of comparison below
				if (DistanceFromCenter > NubRadius)
				{
					_InputState.MousedOverNub = nullptr;
				}
			}

			if (_InputState.MousedOverNub == nullptr)
			{
				bool bNubFound = false;
				// TODO: Only look through the list of VISIBILE nodes (see TODO about occlusion culling)
				for (auto CurNodeIter = _Nodes.begin(); CurNodeIter != _Nodes.end(); CurNodeIter++)
				{
					auto CurNode = *CurNodeIter;

					auto NodePos = CurNode->Position();
					SDL_Rect NodeBounds = CurNode->Graphics().GetBounds();
					NodeBounds.x = (_Camera.ViewBox.w / 2) - _Camera.ViewBox.x + NodePos.x;
					NodeBounds.y = (_Camera.ViewBox.h / 2) - _Camera.ViewBox.y + NodePos.y;

					SDL_Rect NodePlusNubBounds = NodeBounds;
					// TODO: either make a universal nub texture, or replace .x with NubInput texture size
					NodePlusNubBounds.x -= NubOutput::TextureSize().w / 2;
					NodePlusNubBounds.w += NubOutput::TextureSize().w;

					if (SDL_PointInRect(&MousePos, &NodePlusNubBounds))
					{
						//fmt::print("Inside node-nub boundaries.\n");

						// TODO: optimize, consider checking both the left and right halves of the bounding box to check whther it'll be an inpiut or an output that's being moused over.

						for (auto CurDiag : CurNode->Dialogues())
						{
							SDL_Point CurNubPos = CurDiag->Graphics()->NubPoint();
							CurNubPos.x += NodeBounds.x;
							CurNubPos.y += NodeBounds.y;

							// Pythagorean theorem to get the distance from the center of the nub.
							SDL_Point Distances = { CurNubPos.x - MousePos.x, CurNubPos.y - MousePos.y };
							int DistanceFromCenter = sqrt(pow(Distances.x, 2) + pow(Distances.y, 2));

							// Radius = (W - 1) / 2
							const int NubRadius = (NubOutput::TextureSize().w - 1) / 2;

							if (DistanceFromCenter <= NubRadius)
							{
								//fmt::print("Inside a nub!\n");
								_InputState.MousedOverNub = &CurDiag->Nub();
								break;
							}
						}

						if (!bNubFound)
						{
							for (auto CurOpt : CurNode->Options())
							{
								SDL_Point CurNubPos = CurOpt->Graphics()->NubPoint();
								CurNubPos.x += NodeBounds.x;
								CurNubPos.y += NodeBounds.y;

								// Pythagorean theorem to get the distance from the center of the nub.
								SDL_Point Distances = { CurNubPos.x - MousePos.x, CurNubPos.y - MousePos.y };
								int DistanceFromCenter = sqrt(pow(Distances.x, 2) + pow(Distances.y, 2));

								// Radius = (W - 1) / 2
								const int NubRadius = (NubOutput::TextureSize().w - 1) / 2;

								if (DistanceFromCenter <= NubRadius)
								{
									//fmt::print("Inside a nub!\n");
									_InputState.MousedOverNub = &CurOpt->Nub();
									break;
								}
							}
						}
					}

					if (bNubFound)
						break;
				}
			}

			if (Event.user.code & Qt::MouseButton::LeftButton)
			{
				if ((abs(_InputState.DownPosition[0].x - _InputState.Position.x) > QNodeViewInputState::DRAG_THRESHOLD || abs(_InputState.DownPosition[0].y - _InputState.Position.y) > QNodeViewInputState::DRAG_THRESHOLD) 
					&& !_InputState.bDraggingNodes
					&& _InputState.DraggingNub == nullptr) // So the drag can't initiate when a nub is being dragged
				{
					_InputState.bDraggingNodes = true;

					SDL_Point Delta = { _InputState.Position.x - _InputState.DownPosition[0].x, _InputState.Position.y - _InputState.DownPosition[0].y };
					for (auto CurNode : _InputState.SelectedNodes)
					{
						SDL_Point CurNodePos = CurNode->Position();
						CurNodePos = { CurNodePos.x + Delta.x, CurNodePos.y + Delta.y };
						CurNode->SetPosition(CurNodePos);
					}
				}

				// Only drag the selected Nodes if a nub isn't moused over
				if (_InputState.bDraggingNodes == true)
				{
					SDL_Point Delta = { reinterpret_cast<int>(Event.user.data1), reinterpret_cast<int>(Event.user.data2) };
					for (auto CurNode : _InputState.SelectedNodes)
					{
						SDL_Point CurNodePos = CurNode->Position();
						CurNodePos = { CurNodePos.x + Delta.x, CurNodePos.y + Delta.y };
						CurNode->SetPosition(CurNodePos);
					}
				}
			}
			else if (Event.user.code & Qt::MouseButton::MiddleButton)
			{
				SDL_Point Delta = { reinterpret_cast<int>(Event.user.data1), reinterpret_cast<int>(Event.user.data2) };
				//printf_s("%d %d\n", Delta.x, Delta.y);
				_Camera.ViewBox.x -= Delta.x;
				_Camera.ViewBox.y -= Delta.y;
			}
		}
		#pragma endregion
		#pragma region EVENT_MOUSEDOUBLECLICK
		if (Event.type == EVENT_MOUSEDOUBLECLICK)
		{
			SDL_Point DoubleClickPos = GetMousePosition();

			bool bNodeFound = false;
			// TODO: Only look through the list of VISIBILE nodes (see TODO about occlusion culling)
			for (auto CurNode : _Nodes)
			{
				auto NodePos = CurNode->Position();
				auto NodeBounds = CurNode->Graphics().GetBounds();
				NodeBounds.x = (_Camera.ViewBox.w / 2) - _Camera.ViewBox.x + NodePos.x;
				NodeBounds.y = (_Camera.ViewBox.h / 2) - _Camera.ViewBox.y + NodePos.y;

				// A Node is found
				if (SDL_PointInRect(&DoubleClickPos, &NodeBounds))
				{
					if (Event.user.code == Qt::MouseButton::LeftButton)
					{
						SDL_Point PointInsideNode = { DoubleClickPos.x - NodeBounds.x, DoubleClickPos.y - NodeBounds.y };

						bool bHeader = false;
						bool bDlgSection = false;
						bool bOptSection = false;
						NodeDialogue *Dlg = nullptr;
						NodeOption *Opt = nullptr;

						CurNode->FeatureAtPosition(PointInsideNode, bHeader, bDlgSection, bOptSection, &Dlg, &Opt);
						if (bHeader || bDlgSection || bOptSection)
						{
							if (bHeader)
							{
								
							}

							if (bDlgSection)
							{
								this->_Parent->ScriptEditWindow()->dialogueFragment(Dlg);
							}

							return;
						}

						break;
					}
				}
			}
			// No Node is found
		}
		#pragma endregion

		#pragma region SDL Events
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
		#pragma endregion
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

	#pragma region Connection Preview Beziers
	if (_InputState.DraggingNub)
	{
		if (_InputState.DraggingNub->GetNubType() == ANub::NubType::Output)
		{
			NodeOption &CurOpt = static_cast<NubOutput*>(_InputState.DraggingNub)->Parent();
			SDL_Point NubPoint = CurOpt.Graphics()->NubPoint();
			SDL_Rect NodeBounds = CurOpt.Parent().Graphics().GetBounds();
			SDL_Point NodePos = CurOpt.Parent().Position();

			NodeBounds.x = (_Camera.ViewBox.w / 2) - _Camera.ViewBox.x + NodePos.x;
			NodeBounds.y = (_Camera.ViewBox.h / 2) - _Camera.ViewBox.y + NodePos.y;

			NubPoint.x += NodeBounds.x;
			NubPoint.y += NodeBounds.y;

			DrawBezierCurve(NubPoint, _InputState.Position);
		}
		else if (_InputState.DraggingNub->GetNubType() == ANub::NubType::Input)
		{
			NodeDialogue &CurDiag = static_cast<NubInput*>(_InputState.DraggingNub)->Parent();
			SDL_Point NubPoint = CurDiag.Graphics()->NubPoint();
			SDL_Rect NodeBounds = CurDiag.Parent().Graphics().GetBounds();
			SDL_Point NodePos = CurDiag.Parent().Position();

			NodeBounds.x = (_Camera.ViewBox.w / 2) - _Camera.ViewBox.x + NodePos.x;
			NodeBounds.y = (_Camera.ViewBox.h / 2) - _Camera.ViewBox.y + NodePos.y;

			NubPoint.x += NodeBounds.x;
			NubPoint.y += NodeBounds.y;

			DrawBezierCurve(_InputState.Position, NubPoint);
		}
	}
	#pragma endregion

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

		// Draw input nubs
		for (auto Dialogue : Node->Dialogues())
		{
			SDL_Point NubPoint = Dialogue->Graphics()->NubPoint();
			SDL_Rect RenderTgt{ BlockRenderPos.x + NubPoint.x - (ANub::TextureSize().w / 2), BlockRenderPos.y + NubPoint.y - (ANub::TextureSize().h / 2), ANub::TextureSize().w, ANub::TextureSize().h };
			SDL_RenderCopy(Renderer, ANub::TextureDefault(), nullptr, &RenderTgt);
		}

		// Draw output nubs
		for (auto Option : Node->Options())
		{
			SDL_Point NubPoint = Option->Graphics()->NubPoint();
			SDL_Rect RenderTgt{ BlockRenderPos.x + NubPoint.x - (ANub::TextureSize().w / 2), BlockRenderPos.y + NubPoint.y - (ANub::TextureSize().h / 2), ANub::TextureSize().w, ANub::TextureSize().h };
			SDL_RenderCopy(Renderer, ANub::TextureDefault(), nullptr, &RenderTgt);
		}

		//for (auto OutputBox : Node->Graphics().OutputSection()->OutputBoxes())
		//{
		//	auto NubPoint = OutputBox->NubPoint();
		//	SDL_Rect RenderTgt{ BlockRenderPos.x + NubPoint.x - (_Nubs._OutputDefaultSize.w / 2), BlockRenderPos.y + NubPoint.y - /*7*/ (_Nubs._OutputDefaultSize.h / 2), /*9*/ _Nubs._OutputDefaultSize.w, /*15*/ _Nubs._OutputDefaultSize.h };
		//	SDL_RenderCopy(Renderer, _Nubs._OutputDefault, nullptr, &RenderTgt);
		//}
	}

	if (_InputState.MousedOverNub)
	{
		if (_InputState.MousedOverNub->GetNubType() == ANub::NubType::Output)
		{
			NodeOption &CurOpt = static_cast<NubOutput*>(_InputState.MousedOverNub)->Parent();
			SDL_Point NubPoint = CurOpt.Graphics()->NubPoint();
			SDL_Rect NodeBounds = CurOpt.Parent().Graphics().GetBounds();
			SDL_Point NodePos = CurOpt.Parent().Position();

			NodeBounds.x = (_Camera.ViewBox.w / 2) - _Camera.ViewBox.x + NodePos.x;
			NodeBounds.y = (_Camera.ViewBox.h / 2) - _Camera.ViewBox.y + NodePos.y;

			NubPoint.x += NodeBounds.x;
			NubPoint.y += NodeBounds.y;

			SDL_Rect RenderTgt{ NubPoint.x - (ANub::TextureSize().w / 2), NubPoint.y - (ANub::TextureSize().h / 2), ANub::TextureSize().w, ANub::TextureSize().h };
			SDL_RenderCopy(Renderer, ANub::TextureHighlighted(), nullptr, &RenderTgt);
		}
		else if (_InputState.MousedOverNub->GetNubType() == ANub::NubType::Input)
		{
			NodeDialogue &CurDiag = static_cast<NubInput*>(_InputState.MousedOverNub)->Parent();
			SDL_Point NubPoint = CurDiag.Graphics()->NubPoint();
			SDL_Rect NodeBounds = CurDiag.Parent().Graphics().GetBounds();
			SDL_Point NodePos = CurDiag.Parent().Position();

			NodeBounds.x = (_Camera.ViewBox.w / 2) - _Camera.ViewBox.x + NodePos.x;
			NodeBounds.y = (_Camera.ViewBox.h / 2) - _Camera.ViewBox.y + NodePos.y;

			NubPoint.x += NodeBounds.x;
			NubPoint.y += NodeBounds.y;

			SDL_Rect RenderTgt{ NubPoint.x - (ANub::TextureSize().w / 2), NubPoint.y - (ANub::TextureSize().h / 2), ANub::TextureSize().w, ANub::TextureSize().h };
			SDL_RenderCopy(Renderer, ANub::TextureHighlighted(), nullptr, &RenderTgt);
		}
	}

	//SDL_Rect RenderTgt{ 250, 250, 9, 15 };
	//SDL_RenderCopy(Renderer, _Nubs._OutputDefault, nullptr, &RenderTgt);
}

void QNodeView::DrawBezierCurve(const SDL_Point &StartPt, const SDL_Point &EndPt)
{
	#define GetPt(n1, n2, perc) ((int)(n1 + ((n2 - n1) * perc)))

	SDL_Point Ctrl1, Ctrl2;
	if (StartPt.x <= EndPt.x)
	{
		int PtDiff = (EndPt.x - StartPt.x) * 0.10f;

		Ctrl1 = { EndPt.x - PtDiff, StartPt.y };
		Ctrl2 = { StartPt.x + PtDiff, EndPt.y };
	}
	else
	{
		float DiffMult = 1.5f;
		int PtDiff = abs(EndPt.x - StartPt.x);

		Ctrl1 = { StartPt.x + (int)((float)PtDiff * DiffMult), StartPt.y };
		Ctrl2 = { EndPt.x - (int)((float)PtDiff * DiffMult), EndPt.y };
	}

	const size_t NumSegments = 20;
	//size_t NumSegments = std::max(5, std::abs(std::abs(Origin.x - End.y) - std::abs(Origin.y - End.y)) / 100);

	//std::vector<SDL_Point> Segments;
	SDL_Point Segments[NumSegments + 1];
	//Segments.push_back(StartPt);
	Segments[0] = StartPt;

	const float PercentStep = 1.f / (float)NumSegments;
	float Percent = PercentStep;
	for (size_t i = 1u; i <= NumSegments-1; i++)
	{
		SDL_Point PtA{ GetPt(StartPt.x, Ctrl1.x, Percent), GetPt(StartPt.y, Ctrl1.y, Percent) };
		SDL_Point PtB{ GetPt(Ctrl1.x, Ctrl2.x, Percent), GetPt(Ctrl1.y, Ctrl2.y, Percent) };
		SDL_Point PtC{ GetPt(Ctrl2.x, EndPt.x, Percent), GetPt(Ctrl2.y, EndPt.y, Percent) };

		SDL_Point PtM{ GetPt(PtA.x, PtB.x, Percent), GetPt(PtA.y, PtB.y, Percent) };
		SDL_Point PtN{ GetPt(PtB.x, PtC.x, Percent), GetPt(PtB.y, PtC.y, Percent) };

		SDL_Point Dest{ GetPt(PtM.x, PtN.x, Percent), GetPt(PtM.y, PtN.y, Percent) };
		//Segments.push_back(Dest);
		Segments[i] = Dest;
		Percent += PercentStep;
	}
	//Segments.push_back(EndPt);
	Segments[NumSegments] = EndPt;

	/*for (size_t i = 0u; i <= NumSegments; i++)
	{
		DrawThickLine(gra, Color, 2, Segments[i - 1], Segments[i]);
	}*/

	SDL_SetRenderDrawColor(this->SDLRenderer(), 255, 255, 255, 255);
	SDL_RenderDrawLines(this->SDLRenderer(), Segments, NumSegments + 1);

	#undef GetPt
}

QNodeView::QNodeView(QWidget *Parent) :
	QSDLPanel(Parent),
	_FontStore(this->SDLRenderer())
{
	ANub::LoadTextures(this->SDLRenderer());
}

const FontStore &QNodeView::FontStore() const
{
	return _FontStore;
}

void QNodeView::SetDialogueFile(DialogueFile *File)
{
	_DialogueFile = File;
	_Nodes.clear();

	if (File == nullptr)
	{
		// Reset the view and disable the viewbox?
		// Be sure to disable the right-click menu as well.
	}
	else
	{
		_Nodes = _DialogueFile->GetNodes();
	}
}

void QNodeView::SetEngine(ScriptEngine *Engine)
{
	_Engine = Engine;
}

void QNodeView::SetNiirdPad(NiirdPad *NP)
{
	_Parent = NP;
}