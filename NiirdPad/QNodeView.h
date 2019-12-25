#pragma once

#include <vector>

#include <QMenu>

#include "Camera.h"
#include "FontStore.h"
#include "QSDLPanel.h"
#include "ScriptEngine.h"

// DialogueFile.h
class DialogueFile;

// Node.h
class Node;
class ANub;
class NubOutput;
class NodeDialogue;

// NiirdPad.h
class NiirdPad;

class QNodeView : public QSDLPanel
{
protected:
	SDL_Color _GridLineColor = { 50, 50, 50, 0xFF };
	QNodeViewCamera _Camera;
	QNodeViewInputState _InputState;
	FontStore _FontStore;
	// HACK: Figure out a more-elegant solution than this shite.
	ScriptEngine *_Engine = nullptr;
	NiirdPad *_Parent = nullptr;

	DialogueFile *_DialogueFile = nullptr;
	std::vector<Node*> _Nodes;	// Copy of the list from the current DialogueFile. Order changes frequently, most-recently-clicked Node at front.

	virtual void Input() override;
	virtual void Logic() override;
	virtual void Render() override;

	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;

	void RenderBackground();
	void RenderForeground();

	void DrawBezierCurve(const SDL_Point &Start, const SDL_Point &End);

public:
	QNodeView(QWidget *Parent);
	const FontStore &FontStore() const;

	DialogueFile *GetDialogueFile();
	void SetDialogueFile(DialogueFile *File);

	QNodeViewCamera &GetCamera();

	//void JumpTo(ConnectionOutput *OutConn);
	void JumpTo(NodeDialogue *Dlg);

	// HACK: So you can't miss the HACK label for setting a crucial pointer outside the constructor, fuckhead
	void SetEngine(ScriptEngine *Engine);

	NiirdPad *GetNiirdPad();
	// HACK: hee hee hoo hoo ho ho it's ME AGAIN, THE HACK LABEL >:D
	void SetNiirdPad(NiirdPad *NP);

	// HACK: Jesus christ three HACK tags in a row
	void ConnectToReferenceEditWindow();
};