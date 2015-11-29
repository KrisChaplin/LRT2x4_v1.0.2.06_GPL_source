// Copyright (c) 2002-2003 Quadralay Corporation.  All rights reserved.
//

function  HTMLHelpUtility_NotifyClickedPopup()
{
  // Not a member function, need to access via variable
  //
  VarHTMLHelp.mbPopupClicked = true;
}

function  HTMLHelp_Object()
{
  this.mbPopupClicked  = false;
  this.mbOverPopupLink = false;
  this.mEvent          = null;
  this.mPopup          = new HTMLHelpPopup_Object("VarHTMLHelp.mPopup",
                                                  "window",
                                                  HTMLHelpUtility_NotifyClickedPopup,
                                                  "HTMLHelpPopupDIV", "HTMLHelpPopupText",
                                                  10, 12, 20, 400);

  this.fNotifyClicked  = HTMLHelp_NotifyClicked;
  this.fMouseOverPopup = HTMLHelp_MouseOverPopup;
  this.fMouseOutPopup  = HTMLHelp_MouseOutPopup;
  this.fShowPopup      = HTMLHelp_ShowPopup;
  this.fHidePopup      = HTMLHelp_HidePopup;
  this.fPopupDivTag    = HTMLHelp_PopupDivTag;
}

function  HTMLHelp_NotifyClicked()
{
  if (this.mbPopupClicked)
  {
    this.mbPopupClicked = false;
  }
  else if ( ! this.mbOverPopupLink)
  {
    this.fHidePopup();
  }
}

function  HTMLHelp_MouseOverPopup(ParamEvent)
{
  this.mbOverPopupLink = true;

  this.mEvent = new Object();
  this.mEvent.x = ParamEvent.x;
  this.mEvent.y = ParamEvent.y;
}

function  HTMLHelp_MouseOutPopup()
{
  this.mbOverPopupLink = false;
}

function  HTMLHelp_ShowPopup(ParamLink)
{
  var  VarHTML;


  if (this.mEvent != null)
  {
    VarHTML = HTMLHelp_GetPopupHTML(ParamLink);
    if ((VarHTML != null) &&
        (VarHTML.length > 0))
    {
      this.mbPopupClicked = false;

      this.mPopup.fShow(VarHTML, this.mEvent);
    }
  }

  this.mEvent = null;
}

function  HTMLHelp_HidePopup()
{
  this.mPopup.fHide();
}

function  HTMLHelp_PopupDivTag()
{
  return this.mPopup.fDivTagText();
}

function tableRows() {
//alert("table");
 if (document.getElementsByTagName) {
 //alert("if")
  var tables = document.getElementsByTagName('table');
  //alert("length: "+tables.length+" id "+tables.id);
  for (var i = 0; i < tables.length; i++) {
   var trs = tables[i].getElementsByTagName('tr');
   var tds = tables[i].getElementsByTagName('td');
  // alert("tds: "+tds.length);
   for (var j = 1; j < trs.length; j++) {
    if(j%2==0)
	{
     trs[j].className = 'odd';
	 //alert("class: "+trs[j].className)
	}
	else
	{
	 trs[j].className = 'even';
	}
   }
   for (var j = 1; j < tds.length; j++) {
     tds[j].className = 'col';
   }
  }
 }
}
