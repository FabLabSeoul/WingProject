// ********************************************************************************
// **
// ** Helepr Class / macros to implement docking / anchoring for controls within
// ** windows and dialogs. Works with all window classes in ATL, MFC and pure
// ** Win32
// ** 
// ** This source is freeware. In all cases, NO fee must be charged for this piece
// ** of code, for any reason. 
// ** You use this at your own risk !
// **
// ** greetz!, 
// ** A. Thiede / BluePearl Software aka. drice
// **
// ********************************************************************************

// ============================================================================
// USAGE:
//
// - include DECLARE_ANCHOR_MAP() within your window/dialog class declaration
//
// - include the other macros within the implementation file (outside your class)
//
//   example:   BEGIN_ANCHOR_MAP(CMyDialogClass)
//                ANCHOR_MAP_ENTRY(IDOK,         ANF_BOTTOM)
//                ANCHOR_MAP_ENTRY(IDCANCEL,     ANF_BOTTOM)
//                ANCHOR_MAP_ENTRY(IDC_EDITCTRL, ANF_TOP | RIGHT | ANF_BOTTOM)
//                ANCHOR_MAP_ENTRY(NULL,         ANF_AUTOMATIC)
//              END_ANCHOR_MAP()
//
// - After the window has been created (OnInitDialog handler), call InitAnchors();
//
// - Within your WM_SIZE handler, call HandleAnchors and pass the new rectangle
//   of the window (use GetWindowRect() or calculate yourself). You can additionally
//   call Invalidate(false) after calling HandleAnchors to ensure that there are
//   no painting/updating problems when the contorls have been moved.
//
//
// that´s it.
//
// =============================================================================


#pragma once


// anchor/docking flags
// Note: You should not combine docking(ANF_DOCK_...) and anchoring flags.
//       You can try it, but it´s not tested.
//       Additionally, ANF_AUTOMATIC should not be used with any other flags
#define ANF_NONE            0x0000                  /* proportinally moves the control with the size of the window */
#define ANF_DOCK_TOP        0x0001                  /* docks the control to the top of the window */
#define ANF_DOCK_BOTTOM     0x0002                  /* docks the control to the bottom of the window */
#define ANF_DOCK_LEFT       0x0004                  /* docks the control to the left of the window */
#define ANF_DOCK_RIGHT      0x0008                  /* docks the control to the right of the window */
#define ANF_DOCK_ALL        0x000F                  /* docks the control to all sides of the window */    
#define ANF_TOP             0x0010                  /* distance of the control to the top of the window will be constant */
#define ANF_BOTTOM          0x0020                  /* distance of the control to the bottom of the window will be constant */
#define ANF_LEFT            0x0040                  /* distance of the control to the left of the window will be constant */
#define ANF_RIGHT           0x0080                  /* distance of the control to the right of the window will be constant */
#define ANF_AUTOMATIC       0x0100                  /* automatically calculate the anchors, cannot be used with other flags */
#define ANF_DOCK_TOP_EX     0x0200                  /* docks the top of the control to the top of the window */
#define ANF_DOCK_BOTTOM_EX  0x0400                  /* docks the bottom of the control to the bottom of the window */
#define ANF_DOCK_LEFT_EX    0x0800                  /* docks the left-side of the control to the left-side of the window */
#define ANF_DOCK_RIGHT_EX   0x1000                  /* docks the right-side of the control to the right-side of the window */
// some additional control flags
#define ANF_ERASE           0x2000                  /* forces to erase the background of the control in EraseBackground */
// some combinations
#define ANF_TOPLEFT         (ANF_TOP | ANF_LEFT)    
#define ANF_TOPRIGHT        (ANF_TOP | ANF_RIGHT)
#define ANF_BOTTOMLEFT      (ANF_BOTTOM | ANF_LEFT)
#define ANF_BOTTOMRIGHT     (ANF_BOTTOM | ANF_RIGHT)
#define ANF_TOPBOTTOM       (ANF_TOP | ANF_BOTTOM)
#define ANF_LEFTRIGHT       (ANF_LEFT | ANF_RIGHT)
// flags for InitAnchors                    
#define ANIF_CALCSIZE       0x0001                  /* calculate size occupied by all controls, useful for formviews */
#define ANIF_SIZEGRIP       0x0002                  /* add a sizing-grip to the parent window */


class CBPCtrlAnchorMap {

public:

    static const int MAX_CONTROLS   = 256;

    typedef struct FRECT {  // floating-point RECT structure
        double  top;
        double  left;
        double  right;
        double  bottom;
    };
    
    typedef struct FSIZE {  // floating-point SIZE structure
        double  cx;
        double  cy;
    };

    typedef struct TCtrlEntry {
        unsigned int    nCtrlID;    // dialog-control id of the window
        unsigned int    nFlags;     // docking/anchoring flags for this control
        FRECT           rect;       // actual client-rectangle of the control
        HWND            hWnd;       // hWnd of the control
    };
    
protected:
    RECT            m_rcPrev;               // previous rect of the parent window
    BOOL            m_bInitialized;         // class and control-information is inited
    RECT            m_rcNew;                // current window-rect of the parent window
    RECT            m_rcClient;             // current client-rect of the parent window
    unsigned int    m_uiSizedBorders;       // Flags for borders that have been sized
    SIZE            m_szDelta;              // delta of the size-change
    HWND            m_hWndParent;           // handle of the window that contains the controls
    int             m_nCtrlCount;           // number of controls in m_Ctrl
    TCtrlEntry      m_Ctrl[MAX_CONTROLS];   // control-map
    BOOL            m_bUsedDefaultEntry;    // "default" (NULL) anchor-map entry has been used
    unsigned int    m_nDefaultFlags;        // flags for "default" controls
    COLORREF        m_clrBackground;        // background-color of the dialog
    HWND            m_hWndSizeGrip;         // handle of the sizing-grip or NULL
    
public:

  // ======================================================================
  // constructor
  // ======================================================================
  CBPCtrlAnchorMap(void) {

    SetRect(&m_rcPrev, 0, 0, 0, 0);
    m_bInitialized = FALSE;
    SetRect(&m_rcNew, 0, 0, 0, 0);
    SetRect(&m_rcClient, 0, 0, 0, 0);
    m_uiSizedBorders = 0;
    m_szDelta.cx = 0;
    m_szDelta.cy = 0;
    m_hWndParent = NULL;
    m_nCtrlCount = 0;
    m_bUsedDefaultEntry = FALSE;
    m_nDefaultFlags = 0;
    m_clrBackground = GetSysColor(COLOR_BTNFACE);
    m_hWndSizeGrip = NULL;

  };
    
  // ======================================================================
  // destructor
  // ======================================================================
  ~CBPCtrlAnchorMap(void) {
  
  };

  // ======================================================================
  // Adds a control for docking/anchoring. You must call this before calling
  // Initialize() A call to this function is wrapped within the ANCHOR_MAP_ENTRY
  // function. You can pass NULL as nIDCtrl, which affects all other controls
  // that are not currently in the list.
  // ======================================================================
  BOOL AddControl(unsigned int nIDCtrl, unsigned int nFlags) {
  
    if (nIDCtrl==NULL) {
    
      m_bUsedDefaultEntry = TRUE;
      m_nDefaultFlags = nFlags;

      // Note: The entries for the controls are added later within 
      //       the Initialize() function
    
    } else {
  
      if (m_nCtrlCount >= MAX_CONTROLS) return(FALSE);
     
      m_Ctrl[m_nCtrlCount].nCtrlID = nIDCtrl;
      m_Ctrl[m_nCtrlCount].nFlags  = nFlags;
    
      m_nCtrlCount++;
      
    };  

    return(TRUE);
  
  };

  // ======================================================================
  // Returns TRUE if the information for the parent-window and the
  // controls has been initialized, FALSE otherwise
  // ======================================================================
  BOOL IsInitialized(void) {
  
    return(m_bInitialized);
  
  };
  
  // ======================================================================
  // Finds a window with the specified HWND in the control-map and 
  // returns its index or returns -1 if the window was not found
  // ======================================================================
  int FindWindow(HWND hWnd) {
  
    for (int i=0; i < m_nCtrlCount; i++) if (m_Ctrl[i].hWnd==hWnd) return(i);
    return(-1);
  
  };
  
  // ======================================================================
  // SetFREct helper-function
  // ======================================================================
  void SetFRect(FRECT *pRect, double left, double top, double right, double bottom) {
    pRect->left = left;
    pRect->top = top;
    pRect->right = right;
    pRect->bottom = bottom;
  };
  
  
  // ======================================================================
  // ScreenToClient helper-function 
  // ======================================================================
  BOOL ScreenToClient(HWND hWnd, RECT *pRect) {
  
    POINT   pt1;
    POINT   pt2;
    
    pt1.x = pRect->left;
    pt1.y = pRect->top;
    pt2.x = pRect->right;
    pt2.y = pRect->bottom;
    if (::ScreenToClient(hWnd, &pt1)==FALSE) return(FALSE);
    if (::ScreenToClient(hWnd, &pt2)==FALSE) return(FALSE);
    pRect->left = pt1.x;
    pRect->top = pt1.y;
    pRect->right = pt2.x;
    pRect->bottom = pt2.y;
    
    return(TRUE);
  
  };

  // ======================================================================
  // This function does the pre-processing for the calls to HandleAnchors.
  // It stores the new size of the parent-window within m_rcNew, determines
  // which side(s) of the window have been resized and sets the apropriate
  // flags in m_uiSizedBorders and then it calculates the deltas and the
  // new client-rectangle of the parent.
  // The caclculated values are then used by HandleAnchors to move/resize
  // the controls.
  // [in]: pWndRect = new rectangle of the resized parent-window 
  //                  (use GetWndRect())
  // ======================================================================
  void PreProcess(RECT *pWndRect) {

    // do some validation
    _ASSERTE(pWndRect!=NULL);
    if (pWndRect==NULL) return;
    
    // preserve the new bounds of the parent window
    m_rcNew.left = pWndRect->left;
    m_rcNew.top = pWndRect->top;
    m_rcNew.right = pWndRect->right;
    m_rcNew.bottom = pWndRect->bottom;
    
    // determine which sides of the border have changed
    // (we can use our defined ANF_ constants here for simplicity)
    m_uiSizedBorders = 0;
    if (m_rcNew.left!=m_rcPrev.left)        m_uiSizedBorders|=ANF_LEFT;
    if (m_rcNew.top!=m_rcPrev.top)          m_uiSizedBorders|=ANF_TOP;
    if (m_rcNew.right!=m_rcPrev.right)      m_uiSizedBorders|=ANF_RIGHT;
    if (m_rcNew.bottom!=m_rcPrev.bottom)    m_uiSizedBorders|=ANF_BOTTOM;
    
    // calculate deltas
    m_szDelta.cx = (m_rcNew.right-m_rcNew.left)-(m_rcPrev.right-m_rcPrev.left);
    m_szDelta.cy = (m_rcNew.bottom-m_rcNew.top)-(m_rcPrev.bottom-m_rcPrev.top);
    
    // calculate new client-rect
    m_rcClient.right+=m_szDelta.cx;
    m_rcClient.bottom+=m_szDelta.cy;
  
  };

  // ======================================================================
  // This function does the post-processing for the calls to HandleAnchors.
  // It preserves the actual (new) size of the parent-window as "previous
  // size". In the next call to PreProcess, this "previons size" is used
  // to calulate the deltas
  // ======================================================================
  void PostProcess(void) {
  
    m_rcPrev.left = m_rcNew.left;
    m_rcPrev.top = m_rcNew.top;
    m_rcPrev.right = m_rcNew.right;
    m_rcPrev.bottom = m_rcNew.bottom;
  
  };  

  // ======================================================================
  // Child-window enumeration callback function.
  // This function is called from EnumChildWindows, which again is
  // called from Initialize if the "default option" has been used.
  // It adds the enumerated window to the control-list, if it is not
  // already there.
  // ======================================================================
  static BOOL CALLBACK InitDefaultControl(HWND hWnd, LPARAM lParam) {

    CBPCtrlAnchorMap    *pMap = (CBPCtrlAnchorMap *)(lParam);
    int                 iCtrl = 0;
    TCtrlEntry          *pEntry = NULL;
    RECT                rcCtrl;
    
    // do some validation
    if (hWnd==NULL) return(FALSE);
    _ASSERTE(pMap!=NULL);

    // don´t add the child-window if it´s not an immediate child
    // of the parent window. Nested child-windows are moved
    // by their own parents. (The Report-style-ListView-control
    // is such an example)
    if (::GetParent(hWnd)!=pMap->m_hWndParent) return(TRUE);

    // do not add the window if it is already within our list
    for (iCtrl=0; iCtrl < pMap->m_nCtrlCount; iCtrl++) 
      if (pMap->m_Ctrl[iCtrl].hWnd==hWnd) return(TRUE);

    // Add the control
    if (pMap->m_nCtrlCount < MAX_CONTROLS) {
    
      pEntry = &pMap->m_Ctrl[pMap->m_nCtrlCount];
    
      pEntry->hWnd = hWnd;
      pEntry->nCtrlID = ::GetDlgCtrlID(hWnd);
      pEntry->nFlags  = pMap->m_nDefaultFlags;
      ::GetWindowRect(hWnd, &rcCtrl);
      pMap->ScreenToClient(pMap->m_hWndParent, &rcCtrl);

      pEntry->rect.left   = rcCtrl.left;
      pEntry->rect.top    = rcCtrl.top;
      pEntry->rect.right  = rcCtrl.right;
      pEntry->rect.bottom = rcCtrl.bottom;
       
      pMap->m_nCtrlCount++;

    };        
    
    return(TRUE);

  };

  // ======================================================================
  // Initializes the class-members, gets window locations and information
  // about the controls in the control-map (m_Ctrls).
  // dwFlags is a combination of ANIF_ flags
  // ======================================================================
  void Initialize(HWND hWndParent, DWORD dwFlags/*=0*/) {
  
    int     iCtrl = 0;
    HWND    hWndCtrl = NULL;
    RECT    rcCtrl;
    RECT    rcMaxBR;
    SIZE    sz1;
    DWORD   dw1 = 0;
  
    // do some validation
    _ASSERTE(hWndParent!=NULL);
    if (hWndParent==NULL) {
      m_nCtrlCount = 0;
      return;
    };  
    
    rcMaxBR.right = 0;
    rcMaxBR.bottom = 0;
    
    // preserve the handle of the parent window
    m_hWndParent = hWndParent;

    // get information about the parent window
    // which contains the controls
    ::GetWindowRect(m_hWndParent, &m_rcPrev);
    CopyRect(&m_rcNew, &m_rcPrev);
    ::GetClientRect(m_hWndParent, &m_rcClient);
      
    
    // get information about the controls    
    for (iCtrl=0; iCtrl < m_nCtrlCount; iCtrl++) {
    
      hWndCtrl = ::GetDlgItem(m_hWndParent, m_Ctrl[iCtrl].nCtrlID);
      
      _ASSERTE(hWndCtrl!=NULL);
      
      if (hWndCtrl!=NULL) {
      
        m_Ctrl[iCtrl].hWnd = hWndCtrl;
        ::GetWindowRect(hWndCtrl, &rcCtrl);
        ScreenToClient(m_hWndParent, &rcCtrl);
        
        m_Ctrl[iCtrl].rect.left   = rcCtrl.left;
        m_Ctrl[iCtrl].rect.top    = rcCtrl.top;
        m_Ctrl[iCtrl].rect.right  = rcCtrl.right;
        m_Ctrl[iCtrl].rect.bottom = rcCtrl.bottom;
        
      };

    };
    
    // Add the "default entries" to the list, if this option
    // has been used. (Do not usee the following code before all
    // other entries in the control-list have been assigned a
    // valid hWnd.)
    if (m_bUsedDefaultEntry) {

      ::EnumChildWindows(m_hWndParent, InitDefaultControl, (LPARAM)this);

    };
    
    // add the sizing-grip to the parent-window
    if (dwFlags & ANIF_SIZEGRIP) {

      dw1             = m_nDefaultFlags;
      m_nDefaultFlags = ANF_RIGHT | ANF_BOTTOM;
      sz1.cx          = ::GetSystemMetrics(SM_CXVSCROLL);
      sz1.cy          = ::GetSystemMetrics(SM_CYHSCROLL);
      m_hWndSizeGrip  = ::CreateWindow(_T("ScrollBar"), _T(""), WS_CHILD | WS_VISIBLE | SBS_SIZEGRIP, m_rcClient.right-sz1.cx, m_rcClient.bottom-sz1.cy, sz1.cx, sz1.cy, m_hWndParent, NULL, NULL, 0);
      ::SetWindowPos(m_hWndSizeGrip, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOSIZE);
      InitDefaultControl(m_hWndSizeGrip, (LPARAM)this);
      m_nDefaultFlags = dw1;

    };     

    // if bFindEdge!=false, find the bottom-right-most edge to get
    // the initial size of the window    
    if (dwFlags & ANIF_CALCSIZE) {
    
      for (iCtrl=0; iCtrl < m_nCtrlCount; iCtrl++) {
      
        if (m_Ctrl[iCtrl].rect.right > rcMaxBR.right) rcMaxBR.right = (int)m_Ctrl[iCtrl].rect.right;
		if (m_Ctrl[iCtrl].rect.bottom > rcMaxBR.bottom) rcMaxBR.bottom = (int)m_Ctrl[iCtrl].rect.bottom;
      
      };
    
      m_rcPrev.right = m_rcPrev.left+rcMaxBR.right;
      m_rcPrev.bottom = m_rcPrev.top+rcMaxBR.bottom;
      ::AdjustWindowRect(&m_rcPrev, ::GetWindowLong(m_hWndParent, GWL_STYLE), ::GetMenu(m_hWndParent)!=NULL);
      
      m_rcClient.right = m_rcClient.left+(m_rcPrev.right-m_rcPrev.left);
      m_rcClient.bottom = m_rcClient.top+(m_rcPrev.bottom-m_rcPrev.top); 
      
    };  

    // calculate the real flags for the controls, which use
    // the ANF_AUTOMATIC flag
    for (iCtrl=0; iCtrl < m_nCtrlCount; iCtrl++) {
    
      SIZE szClient;
      
      szClient.cx = (m_rcClient.right-m_rcClient.left);
      szClient.cy = (m_rcClient.bottom-m_rcClient.top);
    
      if (m_Ctrl[iCtrl].nFlags==ANF_AUTOMATIC) {
              
        m_Ctrl[iCtrl].nFlags = 0;
        
        // If the top-edge of the control is within the upper-half of the
        // client area, set a top-anchor. If the bottom-edge of the control
        // is within the lower-half of the client area, set a bottom-anchor
        if (m_Ctrl[iCtrl].rect.top   <   (szClient.cy/2)) m_Ctrl[iCtrl].nFlags|=ANF_TOP;
        if (m_Ctrl[iCtrl].rect.bottom >= (szClient.cy/2)) m_Ctrl[iCtrl].nFlags|=ANF_BOTTOM;
        
        // If the left-edge of the control is within the left-half of the
        // client area, set a left-anchor. If the right-edge of the control
        // is within the right-half of the client area, set a right-anchor
        if (m_Ctrl[iCtrl].rect.left  <  (szClient.cx/2)) m_Ctrl[iCtrl].nFlags|=ANF_LEFT;
        if (m_Ctrl[iCtrl].rect.right >= (szClient.cx/2)) m_Ctrl[iCtrl].nFlags|=ANF_RIGHT;
      
      };

    };    
    
    m_bInitialized = TRUE;
  
  };
  


  // ======================================================================
  // Does the actual anchoring/docking processing
  // Note that docking-flags (ANF_DOCK_...) have a higher privilege
  // than the normal anchoring flags. If an ANF_DOCk_... flag is specified
  // for a control, the control is docked and no anchoring is done !
  // [in] : pWndRect = new rectangle of the resized window (use GetWndRect())
  //                   If you pass NULL, the function will call GetWndRect()
  //                   itself.
  //
  // Note: Do not use the nCtrlID-member of the control-map here,
  //       because not every entry may be assigned a valid Control-ID.
  //       Use the hWnd member to access the controls.
  // ======================================================================
  void HandleAnchors(RECT *pRectParent) {
  
    int             iCtrl = 0;
    TCtrlEntry      *pCtrl = NULL;
    FSIZE           szCtrl;
    BOOL            bChanged = FALSE;
    RECT            rcParent;
    HDWP            hDeferPos = NULL;
    WINDOWPLACEMENT wpl;
    
    if (pRectParent==NULL) {
    
      ::GetWindowRect(m_hWndParent, &rcParent);
      PreProcess(&rcParent);
    
    } else PreProcess(pRectParent); 
    
    // handle the visibility of the sizing-grip if we have one
    if (m_hWndSizeGrip!=NULL) {
    
      wpl.length = sizeof(wpl);
      ::GetWindowPlacement(m_hWndParent, &wpl);
      if ((wpl.showCmd==SW_MAXIMIZE) && (::IsWindowVisible(m_hWndSizeGrip))) {
        ::ShowWindow(m_hWndSizeGrip, SW_HIDE);
      } else if (!::IsWindowVisible(m_hWndSizeGrip)) {
        ::ShowWindow(m_hWndSizeGrip, SW_SHOW);
      };
    
    };
    
    // handle child-controls
    hDeferPos = BeginDeferWindowPos(m_nCtrlCount);
  
    for (iCtrl=0; iCtrl < m_nCtrlCount; iCtrl++) {
    
      pCtrl = &m_Ctrl[iCtrl];
      if (pCtrl->hWnd==NULL) continue;
      
      // get the size of the control
      szCtrl.cx = pCtrl->rect.right-pCtrl->rect.left;
      szCtrl.cy = pCtrl->rect.bottom-pCtrl->rect.top;
      
      // we´ve nothing changed until now
      bChanged = FALSE;
      
      // handle docking
      if ((pCtrl->nFlags & ANF_DOCK_ALL)==ANF_DOCK_ALL) {
      
        SetFRect(&pCtrl->rect, 0, 0, m_rcClient.right, m_rcClient.bottom);
        bChanged = TRUE;
      
      } else if (pCtrl->nFlags & ANF_DOCK_TOP) {
      
        SetFRect(&pCtrl->rect, 0.0, 0.0, (double)m_rcClient.right, szCtrl.cy);
        bChanged = TRUE;
      
      } else if (pCtrl->nFlags & ANF_DOCK_BOTTOM) {
      
        SetFRect(&pCtrl->rect, 0, m_rcClient.bottom-szCtrl.cy, m_rcClient.right, m_rcClient.bottom);
        bChanged = TRUE;
      
      } else if (pCtrl->nFlags & ANF_DOCK_LEFT) {
      
        SetFRect(&pCtrl->rect, 0, 0, szCtrl.cx, m_rcClient.bottom);
        bChanged = TRUE;
      
      } else if (pCtrl->nFlags & ANF_DOCK_RIGHT) {
      
        SetFRect(&pCtrl->rect, m_rcClient.right-szCtrl.cx, 0, m_rcClient.right, m_rcClient.bottom);
        bChanged = TRUE;
      
      } else if (pCtrl->nFlags & ANF_DOCK_LEFT_EX) {
      
        SetFRect(&pCtrl->rect, 0, pCtrl->rect.top, szCtrl.cx, pCtrl->rect.bottom);
        bChanged = TRUE;
      
      } else if (pCtrl->nFlags & ANF_DOCK_RIGHT_EX) {
      
        SetFRect(&pCtrl->rect, pCtrl->rect.left, pCtrl->rect.top, m_rcClient.right, pCtrl->rect.bottom);
        bChanged = TRUE;
      
      } else if (pCtrl->nFlags & ANF_DOCK_TOP_EX) {
      
        SetFRect(&pCtrl->rect, pCtrl->rect.left, 0, pCtrl->rect.right, pCtrl->rect.bottom);
        bChanged = TRUE;
      
      } else if (pCtrl->nFlags & ANF_DOCK_BOTTOM_EX) {
      
        SetFRect(&pCtrl->rect, pCtrl->rect.left, pCtrl->rect.top, pCtrl->rect.right, m_rcClient.bottom);
        bChanged = TRUE;
      
      };
      
      // handle anchoring
      if ((m_uiSizedBorders & ANF_LEFTRIGHT) && (m_szDelta.cx!=0) && (!bChanged)) {
      
        switch(pCtrl->nFlags & ANF_LEFTRIGHT) {
        
          case ANF_LEFT         : // nothing to do here, control moves automatically
                                  // with the left-border of the window (client-rect)
                                  break;
          
          case ANF_RIGHT        : pCtrl->rect.left+=m_szDelta.cx;
                                  pCtrl->rect.right=(pCtrl->rect.left+szCtrl.cx);
                                  bChanged = TRUE;
                                  break;
          
          case ANF_LEFTRIGHT    : pCtrl->rect.right+=m_szDelta.cx;
                                  bChanged = TRUE;
                                  break;
          
          default               : pCtrl->rect.left+=((double)m_szDelta.cx/2.0);
                                  pCtrl->rect.right=(pCtrl->rect.left+szCtrl.cx);
                                  bChanged = TRUE;
                                  break;
        
        };
      
      };
      
      if ((m_uiSizedBorders & ANF_TOPBOTTOM) && (m_szDelta.cy!=0)) {
      
        switch(pCtrl->nFlags & ANF_TOPBOTTOM) {
        
            case ANF_TOP        : // nothing to do here, control moves automatically
                                  // with the top of the window (client-rect);
                                  break;  
            
            case ANF_BOTTOM     : pCtrl->rect.top+=m_szDelta.cy;
                                  pCtrl->rect.bottom=(pCtrl->rect.top+szCtrl.cy);
                                  bChanged = TRUE;
                                  break;
                                  
            case ANF_TOPBOTTOM  : pCtrl->rect.bottom+=m_szDelta.cy;
                                  bChanged = TRUE;
                                  break;

            default             : pCtrl->rect.top+=((double)m_szDelta.cy/2.0);
                                  pCtrl->rect.bottom=(pCtrl->rect.top+szCtrl.cy);
                                  bChanged = TRUE;
                                  break;
        };
      
      };

      // now reposition the control, if its size/position has changed        
      if (bChanged!=FALSE) {

        szCtrl.cx = pCtrl->rect.right-pCtrl->rect.left;
        szCtrl.cy = pCtrl->rect.bottom-pCtrl->rect.top;
        ::DeferWindowPos(hDeferPos, pCtrl->hWnd, NULL, (int)pCtrl->rect.left, (int)pCtrl->rect.top, (int)szCtrl.cx, (int)szCtrl.cy, SWP_NOZORDER | SWP_NOACTIVATE);

      };  
    
    };
    
    ::EndDeferWindowPos(hDeferPos);

    PostProcess();
    
  };
  
  // ======================================================================
  // This is an enhanced EraseBackground-function which only erases the
  // background "around" the controls to remove the flicker when the
  // window is resized. Call this function from your OnEraseBackground-
  // (WM_ERASEBACKGROUND)-mesasge handler instead of the default-
  // implementation
  // ======================================================================
  BOOL EraseBackground(HDC hDC) {
  
    HRGN        hRgn1 = NULL;
    HRGN        hRgn2 = NULL;
    HRGN        hRgn3 = NULL;
    RECT        rc;
    HBRUSH      hBrush = NULL;
    int         iCtrl = 0;
    HWND        hWndChild = NULL;
//    char        szClassName[256];
    BOOL        bForceErase = false;
    BOOL        bVisible = false;
  
    _ASSERTE(hDC!=NULL);
    if (hDC==NULL) return(false);

    // create a brush to fill the background with    
    hBrush = CreateSolidBrush(m_clrBackground);
    
    // get the coordinates of the parent-window
    // and create a region-object for the whole
    // area of the window
    ::GetClientRect(m_hWndParent, &rc);
    hRgn1 = CreateRectRgn(rc.left, rc.top, rc.right, rc.bottom);
    hRgn2 = CreateRectRgn(0, 0, 0, 0);
    hRgn3 = CreateRectRgn(0, 0, 0, 0);
    
    // now cycle through all visible controls and 
    // remove their area from 
    hWndChild = ::GetWindow(m_hWndParent, GW_CHILD);
    while (hWndChild!=NULL) {
    
      iCtrl = FindWindow(hWndChild);
      if (iCtrl!=-1) if ((m_Ctrl[iCtrl].nFlags & ANF_ERASE)==0) iCtrl = -1;
      
      bVisible = ::IsWindowVisible(hWndChild);
      
      if ((iCtrl==-1) && (bVisible)) {
    
        ::GetWindowRect(hWndChild, &rc);
        ScreenToClient(m_hWndParent, &rc);        
      
        ::SetRectRgn(hRgn2, rc.left, rc.top, rc.right, rc.bottom);
      
        ::CombineRgn(hRgn3, hRgn1, hRgn2, RGN_DIFF);
      
        HRGN hRgnTemp = hRgn1;
        hRgn1 = hRgn3;
        hRgn3 = hRgnTemp;
        
      };  

      hWndChild = ::GetWindow(hWndChild, GW_HWNDNEXT);
    
    };

    ::FillRgn(hDC, hRgn1, hBrush);

    DeleteObject(hRgn3);
    DeleteObject(hRgn2);
    DeleteObject(hRgn1);
    
    DeleteObject(hBrush);
  
  };

    
};



// ============================================================================
//
// MACROS FOR THE PUBLIC :     
//
// DECLARE_ANCHOR_MAP declares the variable m_bpfxAnchorMap within your window
//                    class and declares the two functions InitAnchors and
//                    HandleAnchors
//
// BEGIN_ANCHOR_MAP   implements the two functions, declared by 
//                    DECLARE_ANCHOR_MAP
//
// ANCHOR_MAP_ENTRY   adds a control to the control-map and pass the 
//                    ID of the control, or NULL for "all other" controls
//
// ANCHOR_MAP_ENTRY_RANGE
//                    This is the same as ANCHOR_MAP_ENTRY except that it
//                    adds a range of controlsto the control-map
//
// END_ANCHOR_MAP     finishes the implementation of InitAnchors and calls
//                    the initialization routine of the helper-class.
//
// ============================================================================

#define DECLARE_ANCHOR_MAP() CBPCtrlAnchorMap m_bpfxAnchorMap; \
                                void InitAnchors(DWORD dwFlags = 0); \
                                void HandleAnchors(RECT *pRect);
                             
#define BEGIN_ANCHOR_MAP(theclass) void theclass::HandleAnchors(RECT *pRect) { \
                                m_bpfxAnchorMap.HandleAnchors(pRect); \
                                }; \
                                void theclass::InitAnchors(DWORD dwFlags) {

#define ANCHOR_MAP_ENTRY(nIDCtrl, nFlags) m_bpfxAnchorMap.AddControl(nIDCtrl, nFlags);

#define ANCHOR_MAP_ENTRY_RANGE(nIDCtrlFrom, nIDCtrlTo, nFlags) \
                                for (int iCtrl=nIDCtrlFrom; iCtrl <= nIDCtrlTo; iCtrl++) \
                                m_bpfxAnchorMap.AddControl(iCtrl, nFlags);

#define END_ANCHOR_MAP()   m_bpfxAnchorMap.Initialize(m_hWnd, dwFlags); \
                           RECT rcWnd;  \
                           ::GetWindowRect(m_hWnd, &rcWnd); \
                           m_bpfxAnchorMap.HandleAnchors(&rcWnd); \
                         };
                             

