//#ident "$Id: Score.cpp,v 1.27 2003/06/18 10:43:44 henqvist Exp $"
/***************************************************************************
                          Score.cpp  -  description
                             -------------------
    begin                : Fri Jan 26 2001
    copyright            : (C) 2001 by Henrik Enqvist
    email                : henqvist@excite.com

    modifs
    20030510  pnf        : Display message ExtraBall when flag activated
                            for current ball.

***************************************************************************/

#include <cstdio>
#include <iostream>
#include <cstring>
#include "Private.h"
#include "Score.h"
#include "Group.h"
#include "Pinball.h"
#include "Keyboard.h"
//#include "SoundUtil.h"
//#include "TextureUtil.h"
#include "EmFont.h"
#include "Config.h"
#include "Table.h"
#include "Loader.h"
#include "Engine.h"
#include "OpenGLVisitor.h"
#include "Menu.h"
#include "Loader.h"

Score::Score() {
  m_Font = EmFont::getInstance();
  this->clear();
}

Score::~Score(){
}

void Score::setText1(const char * text) {
  strncpy(m_Text1, text, 63);
}

void Score::setText2(const char * text) {
  strncpy(m_Text2, text, 63);
}

void Score::setText3(const char * text) {
  strncpy(m_Text3, text, 63);
}

void Score::setText4(const char * text) {
  strncpy(m_Text4, text, 63);
}

void Score::setInfoText(const char * text, int delay) {
  strncpy(m_InfoText, text, 63);
  m_iInfoDelay = delay;
}

void Score::clearText() {
  strcpy(m_Text1, "");
  strcpy(m_Text2, "");
  strcpy(m_Text3, "");
  strcpy(m_Text4, "");
  strcpy(m_InfoText, "");
}


void Score::onTick() {
  EM_COUT("Score::onTick", 0);
  if (m_iInfoDelay < 1) {
    strcpy(m_InfoText, "");
    m_iInfoDelay = -1;
  } else {
    --m_iInfoDelay ;
  }
}

void Score::StdOnSignal() {
  EM_COUT((int)em_signal, 1);

  OnSignal( PBL_SIG_RESET_ALL ) {
    this->clear();
    this->setText1("press enter to launch ball");
    SendSignal( PBL_SIG_CLEAR_TEXT, 400, this->getParent(), NULL );
  } 
  ElseOnSignal( PBL_SIG_TILT ) {
    this->clearText();
    this->setText1("tilt");
    SendSignal( PBL_SIG_RESET_ALL, 600, this->getParent(), NULL );
  }
  ElseOnSignal( PBL_SIG_TILT_WARNING ) {
    this->clearText();
    this->setText1("warning");
    SendSignal( PBL_SIG_CLEAR_TEXT, 400, this->getParent(), NULL );
  }
  ElseOnSignal( PBL_SIG_CLEAR_TEXT ) {
    this->clearText();
  }
  ElseOnSignal( PBL_SIG_BALL_ON ) {
    //this->clearText();
  }
  ElseOnSignal( PBL_SIG_BALL_OFF ) {
    this->m_bExtraBall = false;
  }
  ElseOnSignal( Loader::getInstance()->getSignal("extraball") ) {
    this->m_bExtraBall = true;
  }
  ElseOnSignal( PBL_SIG_GAME_OVER ) {
    this->clearText();

    //cout<<" TODO:give (only) one extra ball if high score"<<endl; //!rzr
    
    if(Config::getInstance()->isHighScoresEnabled() && this->testForHighScore()) {
      this->setText1("last score was a High Score!");
    } else {
      this->setText1("");
    }

    this->setText2("");
    this->setText3("game over");
    this->setText4("press r to start new game");
  }
}

void Score::draw() {
  char buffer[256];
  // Correct bug of ball = 4 at end of game - pnf // !rzr: hum? is it a bug?
  int nCurrentBall = Table::getInstance()->getCurrentBall() + 1;
  if (nCurrentBall < 4) {
    if (m_bExtraBall) {
      sprintf(buffer, "SCORE %d BALL %d ExtraBall", m_iScore, nCurrentBall);
    } else {
      sprintf(buffer, "SCORE %d BALL %d", m_iScore, nCurrentBall);
    }
  } else {
    sprintf(buffer, "SCORE %d", m_iScore);
  }

  m_Font->printRow(buffer, 0);

  if (Config::getInstance()->getShowFPS()) {
    sprintf(buffer, "FPS %.1f"
#if EM_DEBUG
	    "%d"
#endif
	    "\n",
	    Engine::getFps()
#if EM_DEBUG
	    , RenderVisitor::getInstance()->getPolys()
#endif
	    );
    m_Font->printRow(buffer, 1);
  }

  m_Font->printRowCenter(m_Text1, 6);
  m_Font->printRowCenter(m_Text2, 7);
  m_Font->printRowCenter(m_Text3, 8);
  m_Font->printRowCenter(m_Text4, 9);
  m_Font->printRow(m_InfoText, -1);
}

void Score::clear() {
  this->clearText();
  for (int a=0; a<MAX_BALL; ++a) {
    Table::getInstance()->unActivateBall(a);
  }
  Table::getInstance()->setCurrentBall(0);
  m_iInfoDelay = -1;
  m_iScore = 0;
  m_iHiScore = 0;
  m_bExtraBall = false;
}

// Tests for a high score, if test positive asks for the user name
bool Score::testForHighScore() {
  // If it's a high score
  if (Table::getInstance()->isItHighScore(m_iScore)) {
    EmAssert(Engine::getCurrentEngine() != NULL,
	     "Score::testForHighScore Engine NULL");
    //TODO: get : env var  $USER@$HOSTNAME-$(date +%Y%m%d)
    MenuInput input("new highscore! enter name:", Engine::getCurrentEngine());
    input.perform();

    Table::getInstance()->saveNewHighScore(m_iScore, input.getInput());

    return true;
  }
  return false;
}
