/*
 * Copyright (c) 2013 Netklass Tech. Inc. All Rights Reserved.
 *
 * Developed by Netklass Tech. Inc. R&D Software Dept.
 *
 * This file is part of the Netklass Software and may not be distributed,
 * sold, reproduced or copied in any way.
 *
 * This copyright notice should not be removed
 */

/* Common Variable */
var languagetype = 0;
var UI_Version = "v1.10";
var WideLayoutwidth = "92%";	
var NormallLayoutwidth = "85%";
var SmallLayoutwidth = "78%";
var Tableclass = "TableSTA";	
var Tabletileclass="tablestyle";
var Tablealign ="left" ;
var Tableborder = "0" ;
var Tablebordercolor = "#8499A2"; 
var Tablecellspacing = "0";
var Tablecellpadding ="0";
var TableButton = "STbutton";					// Add to list, del add, service management ... 					
var Tablestyle ="border-collapse: collapse";
var Enablelinkcolor ="black";
var Disablelinkcolor ="red";
var Alertcolor="blue";
var Connectlinkcolor ="green";
var Content_title="";
var Content_title1="";
var HasExtendLogo = 0;
var splitVarforSpecialWordField="{[(*-*-*)]}";
var splitVarforSpecialWordField1="{[(***)]}";
var DHCPV6_MAXAVAILABLEIP = 512;
var ConfirmCount=-1;

/***************************************************/
/*												   */	
/*            		 Begin of Tree menu		 	          */
/*												   */	
/***************************************************/
var menuwidth=0;
var menuheigh=0;
var additional_heigh=0;
function CreateNode(UID, link, string, hidden) //constructor 
{ 
	//constant data 
	this.UID = UID
	this.link = link
	this.string = string		
	this.ishidden = hidden  

	//dynamic data 
	this.id = 0;
	this.isSelect = 0   	
	this.children = new Array 
	this.nChildren = 0 

	//methods 
	this.addChild = addChild 
	this.renderOb = drawNode 
} 

function drawNode() 
{ 
	var diffNode = 0;
	if (this.UID !="" && this.ishidden == 0)
	{
		document.write("<tr>\n")
		document.write("<td width=\"200\">\n");
		document.write("<table border=\"0\" cellpadding=\"0\" cellspacing=\"0\" width=\"100%\">\n");
		document.write("<tr>\n ");	  
		if (this.isSelect)
		{				
			if (this.nChildren == 0)
				diffNode = 1;
			else
			{
				if (this.children[0].isSelect == 1 && this.link == this.children[0].link)
					diffNode = 2;
				else
				{
					if (this.children[0].isSelect == 1)
						diffNode = 4;
					else
						diffNode = 5;
					for (var i=1; i<this.nChildren;i++) 
					{
						if (this.children[i].isSelect == 1)
						{							
							diffNode = 3;
							break;
						}
					}
				}
			}		
			if (diffNode == 1)				// No children iterm
			{
				Content_title = this.string;
				document.write("<td class=\"SelectMenu\" width=\"200\" height=\"24\" align=\"left\" valign=\"middle\" onmouseover=\"window.status='"+this.string+"'\" onmouseout=\"window.status=''\"")		
				if (this.link == "")
					document.write(">");	
				else	
					document.write(" onClick=\"javacript:location.href='"+this.link+"'\">");		
				document.write("<img border=\"0\" src=\"images/_blank.gif\" width=\"21\" height=\"12\">")
			}
			else if (diffNode == 2)			// This Node's first children iterm has been select and its link is the same as Node ;
			{
				document.write("<td class=\"SelectedMenu\" width=\"200\" height=\"24\" align=\"left\" valign=\"middle\" onmouseover=\"window.status='"+this.string+"'\" onmouseout=\"window.status=''\">")						
				document.write("<img border=\"0\" src=\"images/_blank.gif\" width=\"4\"><img border=\"0\" src=\"images/cend10.gif\" width=12 height=12><img border=\"0\" src=\"images/_blank.gif\" width=\"5\">")			
			}
			else if (diffNode == 3)			// This Node's children (not first one) has been select
			{
				document.write("<td class=\"SelectedMenu\" width=\"200\" height=\"24\" align=\"left\" valign=\"middle\" onmouseover=\"window.status='"+this.string+"'\" onmouseout=\"window.status=''\">")			
				document.write("<img border=\"0\" src=\"images/_blank.gif\" width=\"4\"><img border=\"0\" src=\"images/cend10.gif\" width=12 height=12><img border=\"0\" src=\"images/_blank.gif\" width=\"5\">")
			}
			else	 if (diffNode == 4)			// This Node's first children iterm has been select but its link isn't the same as Node ;
			{
				document.write("<td class=\"SelectedMenu\" width=\"200\" height=\"24\" align=\"left\" valign=\"middle\" onmouseover=\"window.status='"+this.string+"'\" onmouseout=\"window.status=''\">")		
				document.write("<img border=\"0\" src=\"images/_blank.gif\" width=\"4\"><img border=\"0\" src=\"images/cend10.gif\" width=12 height=12><img border=\"0\" src=\"images/_blank.gif\" width=\"5\">")
			}
			else
			{
				Content_title = this.string;
				document.write("<td class=\"SelectedMenu\" width=\"200\" height=\"24\" align=\"left\" valign=\"middle\" onmouseover=\"window.status='"+this.string+"'\" onmouseout=\"window.status=''\">")		
				document.write("<img border=\"0\" src=\"images/_blank.gif\" width=\"4\"><img border=\"0\" src=\"images/cend10.gif\" width=12 height=12><img border=\"0\" src=\"images/_blank.gif\" width=\"5\">")						
			}
		}
		else
		{
			document.write("<td class=\"UnSelectMenu\" width=\"200\" height=\"30\" align=\"left\" valign=\"middle\" onmouseover=\"this.className='HoverMenu', window.status='"+this.string+"'\" onmouseout=\"this.className='UnSelectMenu'\" ");
			document.write(" onmousedown=\"this.className='PressMenu'\"");
			if (this.link == "")
				document.write(">");	
			else	
				document.write(" onClick=\"javacript:location.href='"+this.link+"'\">");	
			if (this.nChildren > 0)
				document.write("<img border=\"0\" src=\"images/_blank.gif\" width=\"4\"><img border=\"0\" src=\"images/cend30.gif\" width=12 height=12><img border=\"0\" src=\"images/_blank.gif\" width=\"5\">")
			else
				document.write("<img border=\"0\" src=\"images/_blank.gif\" width=\"21\" height=\"12\">")
		}
		if (this.isSelect)
		{
			document.write(this.string)     
			document.write("</b></td>\n")
			document.write("</tr>");
			if (diffNode == 1)
			{
				document.write("</table>\n")
				document.write("</td>\n")
				document.write("</tr>\n")
			}
		}		
		else
		{		
			document.write(this.string) 
			document.write("</td>\n")
			document.write("</tr>");
			document.write("</table>\n")
			document.write("</td>\n")
			document.write("</tr>\n")
		}
		menuheigh += 30;// 24;
	}	
}

// Definition of class Item (a icon or link inside a node) 
// ************************************************************* 
 
function CreateItem(UID,link, string, hidden) // Constructor 
{ 
	//constant data
	this.UID = UID
	this.link = link
	this.string = string 
	
	//dynamic data 
	this.id = 0;
	this.isSelect = 0
	this.ishidden = hidden  	

	//methods 
	this.renderOb = drawItem  
} 

function drawItem() 
{ 
	if (this.ishidden == 0)
	{
		menuheigh += 20;
		document.write("<tr>\n");
		if (this.isSelect)	
		{
			document.write("<td class=\"SelectItem\" height=\"20\" onmouseover=\"window.status='"+this.string+"'\" onmouseout=\"window.status=''\">");
			if (this.link != "")
			{	
				document.write('<a href="'+this.link+'" style="text-decoration:none">');
				document.write("<span style=\"color:#0068d9;background:#fff;\">");
				document.write(this.string);
				document.write("</span>");	
				document.write('</a>');							
			}
			else
			{
				document.write("<span style=\"background:#4086c8;\">");
				document.write(this.string);
				document.write("</span>");	
			}
			Content_title = this.string;
		}	
		else
		{
			document.write("<td class=\"UnSelectItem\" height=\"20\" onmouseover=\"window.status='"+this.string+"';\" onmouseout=\"window.status='';\">");
			if (this.link != "")
			{
				document.write('<a href="'+this.link+'">');
				document.write(this.string);
				document.write('</a>');
			}
			else
			{
				document.write('<img border="0" src="images/_blank.gif" width="10" height="1">'); 
				document.write(this.string);
			}	
		}	
		document.write('</td>\n')  
		document.write("</tr>\n")
	}
	else if (this.isSelect)
		Content_title = this.string;	
}

// Initial selected Node & Item  
function SetItemInit(RootNode, Nodeid, Childid) 
{ 
	var i,j;
	for (i=0 ; i < RootNode.nChildren; i++)  
	{
		if (RootNode.children[i].UID == "")
			continue;
		
		if (RootNode.children[i].UID == Nodeid)
		{
			RootNode.children[i].isSelect = 1
			break;
		}
	}
 	if (Childid>=0 && RootNode.children[i].nChildren > 0)
 	{
		for (j=0 ; j < RootNode.children[i].nChildren; j++)  
		{
			if (RootNode.children[i].children[j].UID == Childid)
			{
				RootNode.children[i].children[j].isSelect = 1
				break;
			}
		}
 	}		
} 

// Change Node link: default or changed 
function SetNodelink(RootNode, Nodeid, link) 
{ 
	var i,j;
	for (i=0 ; i < RootNode.nChildren; i++)  
	{
		if (RootNode.children[i].UID == Nodeid)
		{
			RootNode.children[i].link = link
			break;
		}
	}		
} 

// Change Item link: default or changed 
function SetItemlink(RootNode, Nodeid, Childid, link) 
{ 
	var i,j;
	for (i=0 ; i < RootNode.nChildren; i++)  
	{
		if (RootNode.children[i].UID == Nodeid)
		{
			break;
		}
	}
	
	if (RootNode.children[i].nChildren > 0)
	{	
		for (j=0 ; j < RootNode.children[i].nChildren; j++)  
		{
			if (RootNode.children[i].children[j].UID == Childid)
			{
				RootNode.children[i].children[j].link = link	
				break;
			}
		}
	}		
 	else
		RootNode.children[i].link = link
} 

// Change Item str: default or changed
function SetNodeStr(RootNode, Nodeid, Str) 
{ 
	var i,j;
	for (i=0 ; i < RootNode.nChildren; i++)  
	{
		if (RootNode.children[i].UID == Nodeid)
		{
			RootNode.children[i].string = Str
			break;
		}
	}
}
function SetItemStr(RootNode, Nodeid, Childid, Str) 
{ 
	var i,j;
	for (i=0 ; i < RootNode.nChildren; i++)  
	{
		if (RootNode.children[i].UID == Nodeid)
		{
			break;
		}
	}
	
	if (RootNode.children[i].nChildren > 0)
	{	
		for (j=0 ; j < RootNode.children[i].nChildren; j++)  
		{
			if (RootNode.children[i].children[j].UID == Childid)
			{
				RootNode.children[i].children[j].string = Str
				break;
			}
		}
	}		
 	else
		RootNode.children[i].string = Str
}

// Change Node state: visible or hidden
function SetNodeState(RootNode, Nodeid, State) 
{ 
	var i=0;
	for (i=0 ; i < RootNode.nChildren; i++)  
	{
		if (RootNode.children[i].UID == Nodeid)
		{
			break;
		}
	}	
	RootNode.children[i].ishidden = State
} 

// Change Node state: visible or hidden
function SetItemState(RootNode, Nodeid, Childid, State) 
{ 
	var i,j;
	for (i=0 ; i < RootNode.nChildren; i++)  
	{
		if (RootNode.children[i].UID == Nodeid)
		{
			break;
		}
	}

	for (j=0 ; j < RootNode.children[i].nChildren; j++)  
	{
		if (RootNode.children[i].children[j].UID == Childid)
		{
			RootNode.children[i].children[j].ishidden = State
			break;
		}
	}
}

// Get Selected Node string
function GetSelectedNodelStr(RootNode) 
{ 
	var i,j;
	for (i=0 ; i < RootNode.nChildren; i++)  
	{
		if (RootNode.children[i].isSelect)
		{
			return RootNode.children[i].string;
		}
	}
	return "";
}

// Layout Menu Area
function initializeTree(RootNode, closelink) 
{  
	var i=0;
	var j=0;
	var k=0; //add for change the loop times,belkin new content
	var limit_heigh = 0;
	var LimitHeight = 0;
		
	if (window.innerWidth)
	{ 
		BgHeight = window.innerHeight;
		LimitHeight = 162;		
	}	
	else
	{	
		BgHeight = document.body.clientHeight;
		LimitHeight = 162;		
	}	
	
	document.write("<tr>");
	document.write('<td valign=\"top\"  align="center" style="border-collapse: collapse;width:100%;">');
	PrintHeadLineBar(closelink);
	PrintMainMenu(closelink);
  	if(closelink == "home" || closelink =="wizard" || closelink=="tec")
	{
	  	document.write('<tr>');
		HasExtendLogo = 0;
		menuwidth = 200;
	}
	else
	{
		document.write('<tr>');
		document.write('<td valign="top"  class="contentframe">');
//		document.write('<div class="contback" id="contback">');
		document.write('<div id="menuoverflow" width=\"200\" style="overflow-x:hidden;overflow-y:auto;height:'+(BgHeight -LimitHeight)+'px;">');
		document.write('<table id="MainMenuList" width=\"200\"  valign="top" border="0" cellspacing="0" cellpadding="0" style="background-color:#a5a5a5;" >');
		HasExtendLogo = 0;
		if(closelink=="mainten")
			k=1;
		else 
			k=9;		
		for (i=0 ; i < k; i++)  
		{
			if(closelink&&closelink!="mainten")
				RootNode.children[i].link = "";
			RootNode.children[i].renderOb();
			if (RootNode.children[i].isSelect && RootNode.children[i].ishidden== 0 &&  RootNode.children[i].nChildren > 0)
			{	
				document.write("<tr>\n")
				document.write("<td>\n")
				document.write("<table width=\"200\" border=\"0\" cellspacing=\"0\" cellpadding=\"0\">\n")
				document.write("<tr>\n")
				document.write("<td align=\"left\" valign=\"top\">")
	            		document.write("<table width=\"100%\" border=\"0\" cellspacing=\"0\" cellpadding=\"0\">")
				if (RootNode.children[i].nChildren > 0)
				{
					menuheigh += 4;
					document.write("<tr>");
					document.write("<td class=\"UnSelectItem\" height=\"4\" ></td>");
					document.write("</tr>");
				}			
				for (j=0 ; j < RootNode.children[i].nChildren; j++)
				{
					if(closelink&&closelink!="mainten")
						RootNode.children[i].children[j].link = "";
					RootNode.children[i].children[j].renderOb();
				}	
				if (RootNode.children[i].nChildren > 0)
				{
					menuheigh += 4;
					document.write("<tr>");
					document.write("<td class=\"UnSelectItem\" height=\"4\" ></td>");
					document.write("</tr>");
				}
				document.write("</table>\n")
				document.write("</td>\n")
				document.write("</tr>\n")
				document.write("</table>\n")
				document.write("</td>\n")
				document.write("</tr>\n")
				document.write("</table>\n")
				document.write("</td>\n")
				document.write("</tr>\n")
				document.write("<tr>\n")
				document.write("<td height=\"1\" bgcolor=\"#8499A2\"></td>")
				document.write("</tr>\n")	
				menuheigh += 1;
			}
		}		
		if (window.innerHeight)
			limit_heigh = window.innerHeight - 110;
		else
			limit_heigh = document.body.clientHeight - 109;

		if (menuheigh < limit_heigh)
			additional_heigh = limit_heigh - menuheigh;
		else	
			additional_heigh = 0;	
		document.write('<tr><td><div class="contback" id="contback"></div></td></tr>');
		document.write('</table>');
		document.write('</div>');
		document.write('</td>');
	}
	if(Content_title=="")
		Content_title1 = closelink;
		
	var contback = document.getElementById("contback");	
	if (BgHeight - LimitHeight < menuheigh)
	{
		if(contback)
			contback.style.height = 0;
	}
	else
	{
		if(contback)
			contback.style.height = BgHeight - LimitHeight - menuheigh;//- 21;
	}
}

// Add New Node or Item  
function addChild(childNode) 
{ 
	childNode.id = this.nChildren
	this.children[this.nChildren] = childNode 
	this.nChildren++ 
	return childNode 
} 

// Auxiliary Functions for Node-Treee backward compatibility 
// ********************************************************* 
 
function gFld(ID,link, string,hidden) 
{ 
	Node = new CreateNode(ID, link, string, hidden) 
	return Node 
} 
 
function gLnk(ID,link, string, hidden) 
{  
  	linkItem = new CreateItem(ID, link, string, hidden)   
  	return linkItem 
} 
 
function insFld(parentNode, childNode) 
{ 
  	return parentNode.addChild(childNode) 
} 
 
function insDoc(parentNode, document) 
{ 
  	parentNode.addChild(document) 
} 

//=============== End of Tree menu ================*/

/***************************************************/
/*												     */	
/*            		Begin of Common function             	     */
/*												     */	
/***************************************************/
// Print Background image
function PrintBackground(extend, width)
{
	var application =navigator.appVersion;
	var appType ="";
	var appName = "";
	var appVersion = 0;
	var BgWidth=0;
	var BgHeight=0;
	var LimitHeight = 0;
	if (window.innerWidth)
	{
		BgHeight = window.innerHeight;
		BgWidth = window.innerWidth;
	}
	else
	{
		BgHeight = document.body.clientHeight;
		BgWidth = document.body.clientWidth;
	}
	document.write('<div id="maskAlertDiv" class="divAll">&nbsp;</div><div id="AlertDiv"  style="display:none;position: absolute; width: 356px; top: 30%; left: 40%; background: none repeat scroll 0% 0% white; z-index: 1112;"></div>');
	var i,j=1112;
	for(i=1;i<5;i++)
	{
		document.write('<div id="AlertDiv'+i+'"  style="display:none;position: absolute; width: 356px; top: 30%; left: 40%; background: none repeat scroll 0% 0% white; z-index:'+(i+j)+';"></div>');
	}
	document.write('<div class="page">');
//	document.write('<img border="0" id="bgimg" width="'+BgWidth+'" height="'+BgHeight+'>');//" src="images/body.jpg">');
	document.write('</div>');
	if (extend == "help")
	{
		appType = application.split(";");
		appName = appType[1].substring(1, 5);
		appVersion = parseInt(appType[1].substring(6, appType[1].length),10);
		if (appName == "MSIE" && appVersion < 7)
			document.write('<table border="0" width="100%" style="background:#c0d3e5;"border-color:#53636a;border-width:1px;>');
		else
			document.write('<table class="helptable" border="0" width="100%">');
	}	 
}
// Print Opacity Disable Mask
function PrintMask()
{
	var BgWidth=0;
	var BgHeight=0;
	var a=PrintMask.arguments; 

	if (window.innerWidth)
	{
		BgHeight = window.innerHeight;
		BgWidth = window.innerWidth;
	}
	else
	{
		BgHeight = document.body.clientHeight;
		BgWidth = document.body.clientWidth;
	}
	document.write('<div id="disablemask" style="display:none;">');
	document.write('<table id="masktable" border="'+Tableborder+'" width="'+BgWidth+'" height="'+BgHeight+'" ');
	document.write('style="top: 0px;left: 0px;position: absolute;background-color: #000;opacity:0.75;Filter: Alpha(opacity=75);');
	document.write('width:"'+BgWidth+'"; height:"'+BgHeight+'";');	
	document.write('z-index: 1;>');
	document.write('<tr><td colspan="3" height="30%">&nbsp;</td></tr>');
	document.write('<tr><td width="30%">&nbsp;</td><td align="center" id="ShowProgress">');
   	for(var i=0; i<a.length; i++)
   	{
   		if (i%2 ==0)
   			document.write('<font class="bottomfont">'+a[i]+'</font>');
		else
		{
			document.write('<br><br>');
			document.write('<img src="'+a[i]+'">'); 			
		}
   	}
	document.write('</td><td width="30%">&nbsp;</td></tr>');
	document.write('<tr><td colspan="3" height="35%">&nbsp;</td></tr>');
	document.write('</table>');	
	document.write('</div>');
	/*document.write('<div id="disablemask"  class="WizardDiv" style="top: 65%; left:40%; width: 462px; height: 92px;display:none;">');
    	document.write('<div id="masktable" align="center" style="color: #000000; padding-top:15px; padding-bottom:15px">');
    	document.write('Firmware Upgrade...');
    	document.write('</div>');
	document.write('<div align="">');
	document.write('<img border="0" src="images/loading.gif"/>');
    	document.write('</div>');
	document.write('</div>');*/
}
function ChangeBgsize(width)
{
	var obj;
	var TableWidth = 0;	
	var BgWidth=0;
	var BgHeight=0;
	var ContentWidth = 0;
	var LimitWidth = 0;
	var LimitHeight = 0;
	var Scrobarwidth= 0;
	var isIE = navigator.userAgent.search("MSIE") > -1;
		
	if (window.innerWidth)
	{ 
		//BgWidth = window.innerWidth;
		BgHeight = window.innerHeight;
		//LimitWidth = 202 - menuwidth;
		LimitHeight = 162;//132;		
		Scrobarwidth = 30;		
	}	
	else
	{	
		//BgWidth = document.body.clientWidth;
		BgHeight = document.body.clientHeight;
		//LimitWidth = 203 - menuwidth;
		LimitHeight = 162;//132;	
		Scrobarwidth = 30;		
	}	
	
	obj = document.getElementById("content_block");	
	
	if (BgHeight - LimitHeight < menuheigh + additional_heigh)
		additional_heigh = 0;
	else
		additional_heigh = BgHeight - LimitHeight - menuheigh;
	
	obj = document.getElementById("menuoverflow");	
	if (obj)
		obj.style.height = BgHeight - LimitHeight;
	
	var mnoverflow = document.getElementById("menuoverflow");	
	var contback = document.getElementById("contback");		
	obj = document.getElementById("content_height");	
	var mnframe = document.getElementById("mainframe").style.width;
	var mncontent = parseInt(document.getElementById("mainframe").style.width) - 240;
	if(mnframe == "")
	{
		mnframe = document.getElementById("mainframe").style.width;
		mncontent = document.getElementById("mainframe").style.width
	}
//	if(window.innerWidth)
	if(!isIE)
	{
		if (obj)
		{
			if(parseInt(mnframe) > window.innerWidth)
			{
				//obj.style.width = BgWidth - LimitWidth;
				if(contback)
				{
					if (BgHeight - LimitHeight < menuheigh)
					{			
						obj.style.height = BgHeight - LimitHeight - 22;//obj.style.height = menuheigh;
						obj.style.width = mncontent;
						contback.style.height = 0;
						mnoverflow.style.height = parseInt(menuoverflow.style.height) -22;
					}
					else
					{
						obj.style.height = BgHeight - LimitHeight - 13; //- 21;
						obj.style.width = mncontent;
						contback.style.height = BgHeight - LimitHeight - menuheigh - 22;//- 21;
						mnoverflow.style.height = parseInt(menuoverflow.style.height) -22;
					}
				}
				else
				{
					obj.style.height = BgHeight - LimitHeight - 13;
					obj.style.width = parseInt(mnframe) ;
				}
			}
			else
			{
				if(contback)
				{
					if (BgHeight - LimitHeight < menuheigh)
					{			
						obj.style.height = BgHeight - LimitHeight;//obj.style.height = menuheigh;
						obj.style.width = mncontent;
						contback.style.height = 0;
					}
					else
					{
						obj.style.height = BgHeight - LimitHeight; //- 21;
						obj.style.width = mncontent;
						contback.style.height = BgHeight - LimitHeight - menuheigh;//- 21;
					}
				}
				else
				{
					obj.style.height = BgHeight - LimitHeight + 8;
					obj.style.width = parseInt(mnframe) ;
				}
			}
		}
	}
//	else if(document.body.clientWidth)
	else
	{
		if (obj)
		{
			if(parseInt(mnframe) > document.body.clientWidth)
			{
				//obj.style.width = BgWidth - LimitWidth;
				if(contback)
				{
					if (BgHeight - LimitHeight < menuheigh)
					{			
						obj.style.height = BgHeight - LimitHeight ;//obj.style.height = menuheigh;
						obj.style.width = parseInt(mnframe) - 240;
						contback.style.height = 0;
//						mnoverflow.style.height = parseInt(menuoverflow.style.height) ;
					}
					else
					{
						obj.style.height = BgHeight - LimitHeight ; //- 21;
						obj.style.width = parseInt(mnframe) - 240;
						contback.style.height = BgHeight - LimitHeight - menuheigh;//- 21;
//						mnoverflow.style.height = parseInt(menuoverflow.style.height) ;
					}
				}
				else
				{
					obj.style.height = BgHeight - LimitHeight +8 ;
					obj.style.width = parseInt(mnframe);
				}
			}
			else
			{
				if(contback)
				{
					if (BgHeight - LimitHeight < menuheigh)
					{			
						obj.style.height = BgHeight - LimitHeight;//obj.style.height = menuheigh;
						obj.style.width = mncontent;
						contback.style.height = 0;
					}
					else
					{
						obj.style.height = BgHeight - LimitHeight; //- 21;
						obj.style.width = mncontent;
						contback.style.height = BgHeight - LimitHeight - menuheigh;//- 21;
					}
				}
				else
				{
					obj.style.height = BgHeight - LimitHeight + 8;
					obj.style.width = mnframe;
				}
			}
		}
	}
	obj = document.getElementById("bgimg");	
	if (obj)
	{
		if (BgHeight < menuheigh + additional_heigh + LimitHeight)
			BgHeight = menuheigh + additional_heigh + LimitHeight + 7;
		//obj.style.width= BgWidth;
		obj.style.height= BgHeight;
	}

	obj = document.getElementById("aboutimg");	
	if (obj)
	{
		//obj.style.width= document.body.clientWidth;
		obj.style.height= document.body.clientHeight;
	}
	
	obj = document.getElementById("helpimg");	
	if (obj)
	{
		//obj.style.width= document.body.clientWidth;
		obj.style.height= document.body.clientHeight;
	}

	obj = document.getElementById("masktable");	
	if (obj)
	{
		//obj.style.width= BgWidth;
		obj.style.height= BgHeight;
	}	
	
	return;
}
function PrintWhiteTableBegin()
{
	if (window.ActiveXObject)
		document.write('<div id="whitetable" style="overflow:auto">');		
	else
		document.write('<div id="whitetable" style="overflow:auto">');	
	document.write('<table border="0" width="100%" height="100%" align="center" cellspacing = "0" cellpadding ="0">');
	document.write('<tr>');
       document.write('<td valign="top">');
}
function PrintWhiteTableEnd()
{
	document.write('</td>');
	document.write('</tr>');
	document.write('</table>');
	document.write('</div>');
	ChangeWhiteTable();	
}
function ChangeWhiteTable()
{
	var obj;
	obj = document.getElementById("whitetable");	
	if (obj)
	{
		if (window.innerHeight)
			obj.style.height = window.innerHeight - 45;
		else	
			obj.style.height = document.body.clientHeight - 44;
			
		if (window.innerWidth)
			obj.style.width = window.innerWidth - 16;
		else	
			obj.style.width = document.body.clientWidth - 16;
	}	
}

function callMenuPage(a,b)
{
	MENUNAME = b;

	document.location.href = a;

}

function PrintMainMenu(MENUNAME)
{
	document.write('<tr>');
	document.write('<td>');
	document.write('<table width="100%" border="0" cellspacing="0" cellpadding="0">')
	document.write('<tr>');
	document.write('<td colspan="3"><table width="100%" border="0" cellpadding="0" cellspacing="0" border-collapse="collapse">');
	document.write('<tr height="26">');
	document.write('<td id="summary" name="summary" align="center" class="menustyle1">');
	document.write('<a onClick="JavaScript:callMenuPage(\'home.htm\',1);" >');
	document.write('System Status');
	document.write('</a>');
	document.write('</td>');
//	document.write('<td><img border="0" src="images/_blank.gif" width="1" height="1"></td>');
	document.write('<td id="wizard" name="wizard" align="center" class="menustyle2">');
	document.write('<a onClick="JavaScript:callMenuPage(\'wizard.htm\',2);">');
	document.write('Quick Start');
	document.write('</td>');
//	document.write('<td><img border="0" src="images/_blank.gif" width="1" height="1"></td>');
	document.write('<td id="network" name="network" align="center" width="193" height="40" class="menustyle2">');
	document.write('<a onClick="JavaScript:callMenuPage(\'network.htm\',3);">');
	document.write('Configuration');
	document.write('</a>');
	document.write('</td>');
//	document.write('<td><img border="0" src="images/_blank.gif" width="1" height="1"></td>');
	document.write('<td id="maintenance" name="maintenance" align="center" class="menustyle2">');
	document.write('<a onClick="JavaScript:callMenuPage(\'sys_diag.htm\',4);">');
	document.write('Maintenance');
	document.write('</a>');
	document.write('</td>');
//	document.write('<td><img border="0" src="images/_blank.gif" width="1" height="1"></td>');
	document.write('<td id="tec" name="tec" align="center" class="menustyle2">');
	document.write('<a onClick="JavaScript:callMenuPage(\'support.htm\',5);">');
	document.write('Support');
	document.write('</a>');
	document.write('</td>');
	document.write('</tr>');
	document.write('</table>');
	document.write('</td>');
	document.write('</tr>');
	document.write('<tr>');
	document.write('<td><img border="0" src="images/_blank.gif" width="1" height="8"></td>');
	document.write('<tr>');
	
	if(MENUNAME == "home")
	{
		document.getElementById("summary").className="menustyle1";
		document.getElementById("wizard").className="menustyle2";
		document.getElementById("network").className="menustyle2";
		document.getElementById("maintenance").className="menustyle2";
		document.getElementById("tec").className="menustyle2";
	}
	else if(MENUNAME == "wizard")
	{
		document.getElementById("summary").className="menustyle2";
		document.getElementById("wizard").className="menustyle1";
		document.getElementById("network").className="menustyle2";
		document.getElementById("maintenance").className="menustyle2";
		document.getElementById("tec").className="menustyle2";
	}
	else if(MENUNAME == "tec")
	{
		document.getElementById("summary").className="menustyle2";
		document.getElementById("wizard").className="menustyle2";
		document.getElementById("network").className="menustyle2";
		document.getElementById("maintenance").className="menustyle2";
		document.getElementById("tec").className="menustyle1";
	}
	else if(MENUNAME == "mainten")
	{
		document.getElementById("summary").className="menustyle2";
		document.getElementById("wizard").className="menustyle2";
		document.getElementById("network").className="menustyle2";
		document.getElementById("maintenance").className="menustyle1";
		document.getElementById("tec").className="menustyle2";
	}
	else 
	{
		document.getElementById("summary").className="menustyle2";
		document.getElementById("wizard").className="menustyle2";
		document.getElementById("network").className="menustyle1";
		document.getElementById("maintenance").className="menustyle2";
		document.getElementById("tec").className="menustyle2";
	}
	
}

function setframewidth(s)
{
	var obj = document.getElementById("mainframe");	
	if(obj)
	{
		obj.style.width=s.options[s.selectedIndex].value;
	}
	document.submitform.pagewidthindex.value = s.selectedIndex;
	document.submitform.submit();
}
function PrintHeadLineBar()
{
/*	var isIE = navigator.userAgent.search("MSIE") > -1;
	if(isIE)
	{
		if(document.body.clientWidth>965)
		{
*/
			document.write('<table border="0" id="mainframe" cellpadding="0" cellspacing="0"  height="100%" style="min-width:965px;background:white;border-collapse:collapse;">');
/*
		}
		else if(document.body.clientWidth<=965)
		{
			document.write('<table border="0" id="mainframe" cellpadding="0" cellspacing="0" width="800px" height="100%" style="background:white;border-collapse:collapse;">');
		}

	}
	else
	{
		if(window.innerWidth>965)
		{
			document.write('<table border="0" id="mainframe" cellpadding="0" cellspacing="0" width="965px" height="100%" style="background:white;border-collapse:collapse;">');
		}
		else if(window.innerWidth<=965)
		{
			document.write('<table border="0" id="mainframe" cellpadding="0" cellspacing="0" width="800px" height="100%" style="background:white;border-collapse:collapse;">');
		}
	}	
*/
	document.write('<tr>');	 
	document.write('<td class="logoleft" valign="middle" height="60" colspan="0" padding-left="30px">'); 
   	document.write('<table border="0" cellpadding="0" cellspacing="0" width="100%" style="border-collapse: collapse;">');
	document.write('<tr>');
	document.write('<td valign="bottom" class="leftheaderfont" width="62" rowspan="2">');
	document.write('<div class="Logo" onClick=window.open(\'http://www.belkin.com/\');></div></td>');
//	document.write('<td  class="upperheader"></td>');
	document.write('<td class="rightheaderfont" align="right" valign="bottom">');
	document.write('<img border="0" src="images/_blank.gif" width="20" height="1">');
	document.write('Page Width:');
	document.write('<img border="0" src="images/_blank.gif" width="8" height="1">');
	document.write('<select class="Select" name="pagewidth" id="pagewidth" onchange="setframewidth(this);">');
	if(PageWidth == "0")
		document.write('<option value="" selected>Auto</option>');
	else
		document.write('<option value="">Auto</option>');
		
	if(PageWidth == "1")
		document.write('<option value="965" selected>965px</option>');
	else
		document.write('<option value="965">965px</option>');
	
	if(PageWidth == "2")
		document.write('<option value="1206" selected>1.25*965px</option>');
	else
		document.write('<option value="1206">1.25*965px</option>');
		
	if(PageWidth == "3")
		document.write('<option value="1448" selected>1.5*965px</option>');
	else
		document.write('<option value="1448">1.5*965px</option>');
	
	if(PageWidth == "4")
		document.write('<option value="1668" selected>1.75*965px</option>');
	else
		document.write('<option value="1668">1.75*965px</option>');
		
	if(PageWidth == "5")
		document.write('<option value="1930" selected>2*965px</option>');
	else
		document.write('<option value="1930">2*965px</option>');		
	document.write('</select>');
	document.write('<img border="0" src="images/_blank.gif" width="20" height="1">'+UserName+'');
	document.write('<img border="0" src="images/_blank.gif" width="20" height="1">');
	//if (CloseAllLink == 1);
	//else 
	document.write('<img src="images/icon_logout.gif" width="12" height="12" border="0">&nbsp;<a href="./cgi-bin/welcome.cgi?&status=logout">');
	document.write('Logout</a>');
	/*document.write('<img border="0" src="images/_blank.gif" width="20" height="1"><img src="images/icon_about.gif" width="12" height="12" border="0">&nbsp;');
	document.write('<a href="JavaScript:openTable(\'about.htm\')" title="About">About</a>');*/
	document.write('<img border="0" src="images/_blank.gif" width="20" height="1">');
	document.write('<img src="images/icon_help.gif" width="12" height="12" border="0">&nbsp;<a color="#ffffff" href="JavaScript:callHelp();">Help</a>');
	document.write('</td>');	
    	document.write('</tr>');
	document.write('<tr>');
	document.write('<td valign="bottom" class="downheader" colspan="1">'+ModelName+'&nbsp;&nbsp;'+ModelType+'</td>');
	document.write('</tr>');
  	document.write('</table>');
	document.write('</td>');	
	document.write('</tr>');
	var obj = document.getElementById("pagewidth");
	document.getElementById("mainframe").style.width = obj.options[obj.selectedIndex].value;
}

function PrintBeginBlocktab(hastitle, titlestr, hasSpace)
{
	document.write('<tr><td>');
	if(hastitle=="1")
	{
		document.write('<table id="'+titlestr+'" width="100%" class="Grouptitletab" border="0" cellpadding="0" cellspacing="0">');
	   	document.write('<tr>');
		document.write('<td align="left" valign="top"><font class="Grouptitletab">');	
		if (hastitle == 1)
			PrintBarItem(titlestr, hasSpace);
	   	document.write('</font></td>');
	   	document.write('</tr>');
		document.write('</table>');
		document.write('<table id="'+titlestr+'content" width="98%" class="GroupContenttab" border="0" cellpadding="0" cellspacing="0" style="margin-bottom:0px;">');
	}
	else
	{
		document.write('<table width="98%" class="GroupContenttab" border="0" cellpadding="0" cellspacing="0">');
	}
	document.write('<tr>');
	document.write('<td align="left" valign="top" class="block">');
}

function PrintBeginBlocknewtab(hastitle, titlestr, paddingleft, paddingbottom)
{
	document.write('<tr><td>');
	if(hastitle=="1")
	{
		document.write('<table id="'+titlestr+'" width="100%" class="Grouptitlenewtab" border="0" cellpadding="0" cellspacing="0">');
	   	document.write('<tr>');
		if(paddingleft >= 0)
			document.write('<td align="left" valign="top" style="padding-left:'+paddingleft+'px;"><font class="Grouptitlenewtab">');	
	   	else
			document.write('<td align="left" valign="top"><font class="Grouptitlenewtab">');	
		if (hastitle == 1)
			PrintBarItem(titlestr);
	   	document.write('</font></td>');
	   	document.write('</tr>');
		document.write('</table>');
		document.write('<table id="'+titlestr+'content" width="100%" class="GroupContent" border="0" cellpadding="0" cellspacing="0">');
	}
	else
	{
		document.write('<table width="100%" class="GroupContent" border="0" cellpadding="0" cellspacing="0">');
	}
	document.write('<tr>');
	document.write('<td align="left" valign="top" class="block" style="');
	if(paddingleft >= 0)
		document.write('padding-left:'+paddingleft+'px;');
	if(paddingbottom >= 0)	
		document.write('padding-bottom:'+paddingbottom+'px;');
	document.write('">');
}

function PrintBeginBlock(hastitle, titlestr, hasSpace)
{
	document.write('<tr><td>');
	if(hastitle=="1")
	{
		document.write('<table id="'+titlestr+'" width="100%" class="Grouptitle" border="0" cellpadding="0" cellspacing="0">');
	   	document.write('<tr>');
		document.write('<td align="left" valign="top"><font class="Grouptitle">');	
		if (hastitle == 1)
			PrintBarItem(titlestr, hasSpace);
	   	document.write('</font></td>');
	   	document.write('</tr>');
		document.write('</table>');
/*
		document.write('<table id="'+titlestr+'line" width="100%" class="GroupContent" border="0" cellpadding="0" cellspacing="0">');
	   	document.write('<tr>');
		document.write('<td align="left" valign="top" class="linetd"><br>');
//		document.write('<hr class="line" align="left" size="1">');
	   	document.write('</td>');
	   	document.write('</tr>');
		document.write('</table>');
*/
		document.write('<table id="'+titlestr+'content" width="100%" class="GroupContent" border="0" cellpadding="0" cellspacing="0">');
	}
	else
	{
/*
		document.write('<table  width="100%" class="GroupContent" border="0" cellpadding="0" cellspacing="0">');
	   	document.write('<tr>');
		document.write('<td align="left" valign="top" class="linetd"><br>');
//		document.write('<hr class="line" align="left" size="1">');
	   	document.write('</td>');
	   	document.write('</tr>');
		document.write('</table>');
*/
		document.write('<table width="100%" class="GroupContent" border="0" cellpadding="0" cellspacing="0">');
	}
   	document.write('<tr>');
	document.write('<td align="left" valign="top" class="block">');
}

function PrintEndBlock(hasLine, hasSpace, isSmall)
{
	document.write('</td>');
	document.write('</tr>');
	/*if (hasLine == 1)
		PrintLine(hasSpace, isSmall);*/
	document.write('</table>');
	document.write('</td>');
	document.write('</tr>');	
}

// Print Outline title 
function PrintBarItem(str, hasSpace)
{
	document.write(str);
	//document.write('<br>');
	if (hasSpace == 1) document.write('<img border="0" src="images/_blank.gif" width="1" height="10">'); 
	else document.write('<img border="0" src="images/_blank.gif" width="1" height="15">');
}

// Print bar-line
function PrintLine(hasSpace, isSmall)
{
	if (isSmall == 1)
	{
		if (hasSpace == 1);
		else
			document.write('<tr><td colspan="2" height="5"></td></tr>');
		document.write('<tr><td colspan="2">');
		document.write('<hr class="line" align="center" size="1" width="100%">');        	
		document.write('</td>');
		document.write('</tr>');
	}
	else
	{
		if (hasSpace == 1);
		else
			document.write('<tr><td colspan="2" height="9"></td></tr>');
		document.write('<tr><td colspan="2">');
		document.write('<hr class="line" align="center" size="1" width="100%">');        	
		document.write('</td>');
		document.write('</tr>');
		document.write('<tr><td colspan="2"><img border="0" src="images/_blank.gif" width="1" height="3"></td></tr>');
	}
}

function PrintBeginContent()
{
	var obj;
	var i=0;
	var j=0;
	var application =navigator.appVersion;
	var appType ="";
	var appName = "";
	var appVersion = 0;
	var BgWidth = 0;
	var BgHeight = 0;
	var Tablewidth = 0;
	var ContentWidth = 0;
	var ContentHight = 0;
	var LimitWidth =0;
	var LimitHeight =0;
	var bodywidth;
	if(document.getElementById("MainMenuList"))
		bodywidth = parseInt(document.getElementById("mainframe").style.width) - 240;
	else
		bodywidth = document.getElementById("mainframe").width;
	if (window.innerWidth)
	{
		BgWidth = window.innerWidth;
		BgHeight = window.innerHeight;
		LimitWidth = 202 - menuwidth;
		LimitHeight = 162;//132;
	}	
	else
	{
		BgWidth = document.body.clientWidth;
		BgHeight = document.body.clientHeight; 
		LimitWidth = 203 - menuwidth;
		LimitHeight = 162;//132;
	}	
	
	if (BgWidth < 450)
	{
		BgWidth = 450;
		Tablewidth = 425;
	}	
	else if (BgWidth < screen.availWidth)
		Tablewidth = BgWidth - 30;
	else
		Tablewidth = screen.availWidth - 30;
	
	ContentWidth = BgWidth - LimitWidth - 18;
	if (ContentWidth < 700)
		ContentWidth = 700;
	
	if (HasExtendLogo == 1)
		ContentWidth += 18;

	/*if (BgHeight - LimitHeight < menuheigh)
		ContentHight = menuheigh;
	else*/
		ContentHight = BgHeight - LimitHeight ;//-35;
		
	obj = document.getElementById("bgimg");	
	if (obj)
	{
		obj.style.width = BgWidth;
		obj.style.height = BgHeight;	
	}	
		
	if(application.lastIndexOf(";") != -1)
	{
	    appType = application.split(";");
	    appName = appType[1].substring(1, 5);
	    appVersion = parseInt(appType[1].substring(6, appType[1].length),10);
	}
	else
	{
	    appName="";
	    appVersion=0;
	}
	if (appName == "MSIE")
		document.write('<td valign="top"');
	else
		document.write('<td valign="top" class="contentbg"');	
	document.write(' width="100%">');
//	document.write('<div id="content_height" style="overflow-x:auto;overflow-y:auto;width:'+(bodywidth)+'px;height:'+ContentHight+'px;">');
	if(document.getElementById("contback"))
	{
		//document.getElementById("contback").style.height = ContentHight - menuheigh;
		document.write('<div id="content_height" style="overflow-x:hidden;overflow-y:auto;width:'+(bodywidth)+'px;height:'+ContentHight+'px;">');
	}
	else
	{
		document.write('<div id="content_height" style="overflow-x:hidden;overflow-y:auto;width:'+(bodywidth)+'px;height:'+(ContentHight+8)+'px;">');
	}
	document.write('<table id="content_block" width="98%">');
	document.write('<tr>');
	document.write('<td>');
	document.write('<table width="100%" border="0" style="'+Tablestyle+'">');
	document.write('<tr>');		
	if(Content_title=="")
	{
		if(Content_title1=="home")
		{
			document.write('<td class="Contenttitle1">');
			document.write("System Status");
		}
		else if(Content_title1=="wizard")
		{
			document.write('<td class="Contenttitle1">');
			document.write("Wizard");
		}
		else if(Content_title1=="tec")
		{
			document.write('<td class="Contenttitle1">');
			document.write("Technical Support");
		}
		else	
			document.write('<td>');
	}
	else
	{
		document.write('<td class="Contenttitle">');
		document.write(Content_title);
	}
	document.write('</td>');
	document.write('</tr>');
	document.write('</table>');
	document.write('</td>');
   	document.write('</tr>');
	document.title = "Linksys " + ModelName + " Configuration Utility";
}

function PrintEndContent()
{
	document.write('</table>');
	document.write('<table width="'+NormallLayoutwidth+'" border="0">');
	document.write('<tr>');
	document.write('<td height="5"></td>');
   	document.write('</tr>');	
	document.write('<tr><td style="padding-top:0px; padding-bottom:0px;">');
	PrintButton();
	document.write('</td></tr>');
	document.write('<tr>');
	document.write('<td height="8"></td>');
   	document.write('</tr>');		
	document.write('</table>');
	document.write('</div>');
	document.write('</td>');
	document.write('</form>');
    document.write('</tr>');	
}

function PrintEndPopwindow()
{
	document.write('<table width="100%" border="0" cellpadding="0">');
	document.write('<tr><td align="left" style="padding-top:10px;margin-bottom:0px;">');
	PrintButton();	
	document.write('</td></tr>');
	if (window.innerHeight);
	else
	{
		document.write('<tr>');
		document.write('<td height="8" colspan="2"></td>');
   		document.write('</tr>');	
	}
	document.write('</table>');
}
function PrintCloseButton(HelpPage)
{
	document.write('<div align="center">');
	if (HelpPage)
		PrintSTButton("CloseWindow", "Close", "closeWindow()", 1, "", 70);
	else	
		PrintSTButton("CloseWindow", "Close", "closeWindow()", 0, "", 70);
	document.write('</div>');
}
function PrintRight()
{
}

function PrintBottom()
{
/*	document.write('<tr>');	
	document.write('<td colspan="3" height="14" style="background-color:#303030;"></td>');
	document.write('</tr>');*/
	document.write('<tr align="right">');	
	document.write('<td valign="top" align="right" width="100%" class="bottomfont" colspan="3"  height="35px" style="vertical-align:middle;">&copy; 2013 Belkin International, Inc. and/or its subsidiaries and affiliates, including Linksys, LLC. All rights reserved.</td>')
	document.write('</tr>');
  	document.write('</table>');
}

function ChangeInputStyle()
{
	var isIE = navigator.userAgent.search("MSIE") > -1;
	var obj = document.getElementsByTagName("input");
	var i;
	if(isIE)
	{
		for(i=0;i<obj.length;i++)
		{
			if(obj[i].type=="text"||obj[i].type=="password")
			{
				obj[i].className="IEinputtext";
			}
		}
	}
	else
	{
		for(i=0;i<obj.length;i++)
		{
			if(obj[i].type=="text"||obj[i].type=="password")
			{
				obj[i].className="inputtext";
			}
		}
	}
}

function PrintButton()
{
	switch(ButtonType)
	{
		case 'Refresh':
			PrintRefreshButton(Reloadlink);
			break;
		case 'RefreshClose':	
			PrintRefreshCloseButton(Reloadlink);
			break;				
		case 'OkCancel':
			PrintOKCancelButton(Savelink, Cancellink);
			break;
		case 'OkCancelClose':	
			PrintOKCancelCloseButton(Savelink, Cancellink);
			break;	
		case 'OkResetClose':	
			PrintOKResetCloseButton(Savelink, Cancellink);
			break;
		case 'BackOkCancel':
			PrintBackOKCancelButton(Backlink, Savelink, Cancellink);
			break;	
		case 'ShowTableOkCancel':
			PrintShowTableOKCancelButton(Tablelink, Savelink, Cancellink);
			break;		
		default:
			break;
	}
}
/****************************************************/
/*												      */	
/* 		           Submit Button            	  			      */
/*												      */	
/***************************************************/
function GetReload()
{
	var href = "javascript: location.";
	var UrlString = document.location.href;
	var fileindex = 0, pathindex = 0;
	var tmpurl = "";

	fileindex = UrlString.lastIndexOf(".htm", UrlString.length);
	if (fileindex != -1)
		pathindex = UrlString.lastIndexOf("/", fileindex);

	if (pathindex != -1)
		tmpurl = UrlString.substring(pathindex+1, fileindex);

	if (tmpurl)
		href += "href='"+tmpurl+".htm';";
	else	
		href += "reload();"

	return href;
}

function GetURLParam()
{
	var param = "";
	var UrlString = document.location.href;
	var fileindex = 0;

	fileindex = UrlString.lastIndexOf(".htm#", UrlString.length);
	if (fileindex != -1)
		param = UrlString.substring(fileindex+5, UrlString.length);
		
	return param;
}

function PrintRefreshButton(RefreshStr)
{
	document.write('<table border="0" cellpadding="0" cellspacing="0"  valign="bottom" id="AutoNumber15" height="21">');
	document.write('<tr>'); 
	document.write("<td height=\"26\">");
	PrintSTButton("Refresh", "Refresh", "javascript: location.reload();", "", "", 75);
	document.write("</td>");
	document.write('</tr>');
	document.write('</table>');
}

function PrintRefreshCloseButton(RefreshStr)
{
	document.write('<table border="0" cellpadding="0" cellspacing="0"  valign="bottom" id="AutoNumber15" height="21">');
	document.write('<tr>'); 
	document.write("<td height=\"26\">");
	PrintSTButton("Refresh", "Refresh", "javascript: location.reload();", "", "", 75);
	document.write("</td>") 
	document.write('<td width="5"></td>');
	document.write("<td>");
	PrintSTButton("Close", "Close", "javascript: closeWindow();", "","", 75);
	document.write("</td>"); 
	document.write('</tr>');
	document.write('</table>');
}

function PrintOKCancelButton(YesStr, CancelStr)
{
	var Savelink=YesStr;
	document.write('<table border="0" cellpadding="0" cellspacing="0"  valign="bottom" id="AutoNumber15" style="'+Tablestyle+'">');
	document.write('<tr>'); 
	document.write("<td height=\"26\">");
	PrintSTButton("Save", "Save", "chAll_Words();", "", "submit", 75);
	document.write("</td>") 
	document.write('<td width="5"></td>');
	document.write("<td>");
	if (CancelStr  != "")
		PrintSTButton("Cancel", "Cancel", CancelStr, "", "", 75);
	else	
		PrintSTButton("Cancel", "Cancel", GetReload(), "", "", 75);
	document.write("</td>");
	document.write('</tr>');
	document.write('</table>');
}
function PrintOKCancelCloseButton(YesStr)
{
	var Savelink=YesStr;
	document.write('<table border="0" cellpadding="0" cellspacing="0"  valign="bottom" id="AutoNumber15" height="21">');
	document.write('<tr>'); 
	document.write("<td height=\"26\">");
	PrintSTButton("OK", "OK", "chAll_Words();", "", "submit", 75);
	document.write("</td>") 
	document.write('<td width="5"></td>');
	document.write("<td>");
	document.write("<td>");
	PrintSTButton("Cancel", "Cancel", GetReload(), "", "", 75);
	document.write("</td>") 
	document.write('<td width="5"></td>');
	document.write("<td>");
	PrintSTButton("Close", "Close", "javascript: closeWindow();", "", "", 75);
	document.write("</td>"); 
	document.write('</tr>');
	document.write('</table>');
}
function PrintOKResetCloseButton(YesStr)
{
	var Savelink=YesStr;
	document.write('<table border="0" cellpadding="0" cellspacing="0"  valign="bottom" id="AutoNumber15" height="21">');
	document.write('<tr>'); 
	document.write("<td height=\"26\">");
	PrintSTButton("OK", "OK", "chAll_Words();", "", "submit", 75);
	document.write("</td>") 
	document.write('<td width="5"></td>');
	document.write("<td>");
	document.write("<td>");
	PrintSTButton("Reset", "Reset", "", "", "reset", 75);
	document.write("</td>") 
	document.write('<td width="5"></td>');
	document.write("<td>");
	PrintSTButton("Close", "Close", "javascript: closeWindow();", "", "", 75);
	document.write("</td>"); 
	document.write('</tr>');
	document.write('</table>');
}
function PrintBackOKCancelButton(BackStr,YesStr, CancelStr)
{
	var Savelink=YesStr;
	document.write('<table border="0" cellpadding="0" cellspacing="0"  valign="bottom" id="AutoNumber15" height="21">');
	document.write('<tr>');
	/*
	document.write("<td height=\"26\">");
	if (BackStr  == "")
		PrintSTButton("Refresh", "Refresh", "javascript: location.reload();", "", "", 75);
	else	
		PrintSTButton("Back", "Back", "javascript:location.href='"+BackStr+"'", "", "", 75);
	document.write("</td>") 
	document.write('<td width="5"></td>');
	*/
	document.write("<td>");	
	document.write("<td>");
	PrintSTButton("Save", "Save", "chAll_Words();", "", "submit", 75);
	document.write("</td>") 
	document.write('<td width="5"></td>');
	document.write("<td>");
	if (CancelStr  != "")
		PrintSTButton("Cancel", "Cancel", CancelStr, "", "", 75);
	else	
		PrintSTButton("Cancel", "Cancel", "javascript:location.href='"+BackStr+"'", "", "", 75);
	document.write("</td>");
	document.write('</tr>');
	document.write('</table>');
}

function PrintShowTableOKCancelButton(TableStr,YesStr, CancelStr)
{
	document.write('<table border="0" cellpadding="0" cellspacing="0"  valign="bottom" id="AutoNumber15" height="21">');
	document.write('<tr>'); 
	document.write("<td height=\"26\">");
	if (TableStr != "")
		PrintSTButton("View", "View", "javascript:"+TableStr, "", "", 75);
	else
		PrintSTButton("Refresh", "Refresh", GetReload(), "", "", 75);
	document.write("</td>") 
	document.write('<td width="5"></td>');
	document.write("<td>");	
	document.write("<td>");
	PrintSTButton("Save", "Save", "chAll_Words();", "", "submit", 75);
	document.write("</td>") 
	document.write('<td width="5"></td>');
	document.write("<td>");
	if (CancelStr  != "")
		PrintSTButton("Cancel", "Cancel", CancelStr, "", "", 75);
	else	
		PrintSTButton("Cancel", "Cancel", GetReload(), "", "", 75);
	document.write("</td>");
	document.write('</tr>');
	document.write('</table>');
}
function PrintPageButton(Type, clickfun, status)
{
	var normal_graph = "_blank.gif";
	var hover_graph = "_blank.gif";
	var press_graph = "_blank.gif";
	
	switch (Type)
	{
		case "first":
			normal_graph = "nb.gif";
			hover_graph = "hb.gif";
			press_graph = "pb.gif";
			break;
		case "previous":
			normal_graph = "nl.gif";
			hover_graph = "hl.gif";
			press_graph = "pl.gif";
			break;	
		case "next":
			normal_graph = "nr.gif";
			hover_graph = "hr.gif";
			press_graph = "pr.gif";
			break;
		case "end":
			normal_graph = "nn.gif";
			hover_graph = "hn.gif";
			press_graph = "pn.gif";
			break;	
		default:
			break;
	}
	document.write('<img border="1" src="images/'+normal_graph+'" width="16" height="16" style="border-color:#53636A"');
	if (status && status == "disabled") document.write('>');
	else
	{
		document.write(' onmouseover="this.src=\'images/'+hover_graph+'\';this.style.borderColor=\'#1FA0D5\'"');
		document.write(' onmouseout="this.src=\'images/'+normal_graph+'\'; this.style.borderColor=\'#53636A\'"');
		document.write(' onmousedown="this.src=\'images/'+press_graph+'\';this.style.cursor=\'pointer\';this.style.color=\'#1FA0D5\';\"');
		document.write(' onmouseup="this.src=\'images/'+normal_graph+'\'; this.style.borderColor=\'#53636A\'"');
		document.write(' onClick="'+clickfun+'" >');
	}
}

function PrintSTButton(Name, StrValue, ClickFun, haspath, type, fixedwidth)
{	
	if(StrValue == "Cancel")
		document.write('<input type="button" class="SecSTbutton" name="'+Name+'" id="'+Name+'" value="'+StrValue+'"');
	else if (type && type == "reset")
		document.write('<input type="reset" class="STbutton" name="'+Name+'" id="'+Name+'" value="Cancel"');
	else
		document.write('<input type="button" class="STbutton" name="'+Name+'" id="'+Name+'" value="'+StrValue+'"');
	
	 
	document.write(' onmouseover="ChangeButtonStyle(\''+StrValue+'\',this,\'mouseover\'');
	if (haspath)	 document.write(', '+haspath+')"');
	else	 document.write(')"');		
	document.write(' onmouseout="ChangeButtonStyle(\''+StrValue+'\',this,\'mouseout\'');
	if (haspath)	 document.write(', '+haspath+')"');
	else	 document.write(')"');	
	document.write(' onmousedown="ChangeButtonStyle(\''+StrValue+'\',this,\'mousedown\'');
	if (haspath)	 document.write(','+haspath+')"');
	else	 document.write(')"');	
	document.write(' onmouseup="ChangeButtonStyle(\''+StrValue+'\',this,\'mouseup\'');
	if (haspath)	 document.write(','+haspath+');"');
	else	 document.write(')"');	
	if (ClickFun) document.write(' onClick="'+ClickFun+'"');	
	document.write(' style="');
	if (fixedwidth)	
		document.write('width:'+fixedwidth+'px;');
	if (type && type == "submit")
		document.write('border-width:1px;border-style:outset;');
	else
		document.write('border-width:1px;border-style:outset;');
	document.write('" >');	
}
function ChangeButtonStyle(Name,obj, EventType, haspath)
{
	if(EventType == 'mouseover')
	{
		obj.style.color ="#FFF";		
		obj.style.background ="#64798f";
		//obj.style.borderColor ="#1FA0D5";
		//if (haspath) obj.style.background ="url('../images/BTN1.gif')";
		//else obj.style.background ="url('images/BTN1.gif')";
	}
	else if(EventType == 'mouseout')
	{
		window.status = "";
		obj.style.color ="#FFF";
		//obj.style.borderColor ="#53636A";
		if(Name == "Cancel")
			obj.style.background ="#7d7d7d";
		else
			obj.style.background ="#70a0d4";
		//else obj.style.background ="url('images/BTN0.gif')";
	}
	else if(EventType == 'mousedown')
	{
		obj.style.color ="#FFF";
		//obj.style.borderColor ="#8499a2";
		obj.style.background ="#0068d9";
		//if (haspath) obj.style.background ="url('../images/BTN3.gif')";
		//else obj.style.background ="url('images/BTN3.gif')";
	}
	else if(EventType == 'mouseup')
	{
		obj.style.color ="#FFF";
		//obj.style.borderColor ="#1FA0D5";
		if(Name == "Cancel")
			obj.style.background ="#7d7d7d";
		else
			obj.style.background ="#70a0d4";
		//if (haspath) obj.style.background ="url('../images/BTN1.gif')";
		//else obj.style.background ="url('images/BTN1.gif')";
	}	
}	
function SetButtonStatus(Name, StatusType)
{
	var obj = document.getElementById(Name);
	if (StatusType == 'disabled')
	{
		obj.disabled = true;
		obj.style.borderColor ="#BBC2C5";
//		obj.style.background ="url('images/BTN4.gif')";
		obj.style.visibility ="visible";
		obj.style.color= "#fff";
//		obj.style.color= "#8E8E8E";
		obj.style.cursor="default";
	}
	else if (StatusType == 'hidden')
	{
		obj.disabled = false;
		obj.style.borderColor ="#BBC2C5";
//		obj.style.background ="url('images/BTN4.gif')";
		obj.style.visibility ="hidden";
		obj.style.color= "#fff";
//		obj.style.color="#8E8E8E";
	}	
	else
	{
		obj.disabled = false;
		obj.style.borderColor ="#53636A";
//		obj.style.background ="url('images/BTN0.gif')";
		obj.style.visibility ="visible";
		obj.style.color= "#fff";
//		obj.style.color = "#000";
		obj.style.cursor="pointer";
	}	
}

function ConfirmMsg(title, msg , mode , a , b , c)//mode="chSubmit"||"chSubmit2"
{	
	ConfirmCount = (++ConfirmCount);
	if(ConfirmCount==0)
	{
		var confirmdiv="AlertDiv";
	}
	else
	{
		var confirmdiv="AlertDiv"+ConfirmCount.toString();
	}
	var maskDivObj = document.getElementById ('maskAlertDiv');
	var popDivObj = document.getElementById (confirmdiv);


	maskDivObj.style.display = "block";
	popDivObj.style.display = "block";
	var HtmlStr="";
	HtmlStr+='<table width="100%" border="0" cellspacing="0" cellpadding="0" align="center" style="border:1px solid #fff;background-color:#f9f9f9;">'; //background:url(\'images/opacity67.png\');
	HtmlStr+='<tr >';
	HtmlStr+='<td width="100%" title="error" style="cursor:move;text-align:center;background-color:#7d7d7d;height:40px;">'; 
	HtmlStr+='<font style="font-size:18px;font-family:Arial Reg, Helvetica, sans-serif;color:#fff;">'+title+'</font></td> ';
/*	HtmlStr+='<td width="30"> ';
	HtmlStr+='<img src="images/close.gif" style="margin-right:3px;cursor:hand;" onclick="alertOk();" onmouseover="ChangeStyle(this,1);" onmouseout="ChangeStyle(this,2)"><td>';*/
	HtmlStr+='</tr>';
	HtmlStr+='<tr   style="padding:2px;">';
	HtmlStr+='<td >';
    HtmlStr+='<table width="100%" border="0" cellspacing="0" cellpadding="0" align="center" style="margin:2px;">'; 
	HtmlStr+='<tr bgcolor="#f9f9f9"><td style="padding-right:20px;padding-left:20px;padding-bottom:0px">';
	HtmlStr+='<table width="100%">';
	HtmlStr+='<tr><td >';
	HtmlStr+='<img src="images/icon_alert.gif" style="background:#f9f9f9;"></td>';
	HtmlStr+='<td align="left" style="color:#303030;font-family:Arial Reg,Helvetica,sans-serif;font-size:13px;padding:15px;">'+msg+'</td></tr><tr><td colspan="2" style="font-size:2px;"><hr style="color:#e2e2e2;margin: 0px; border-width: 1px;"></td></tr>';  
	HtmlStr+='<tr ><td colspan="2" style="background:#f9f9f9;padding-top:15px;padding-bottom:15px;" valign="top" align="center">'; 
	HtmlStr+='<input type="button" style="margin-right:20px;width: 75px; border-width: 1px; border-style: outset; color:#ffffff; background: none repeat scroll #70A0D4;" onclick="ConfirmMsgYes(\''+mode+'\',\''+a+'\',\''+b+'\',\''+c+'\');" onmouseup="ChangeButtonStyle(\'Save\',this,\'mouseup\')" onmousedown="ChangeButtonStyle(\'Save\',this,\'mousedown\')" onmouseout="ChangeButtonStyle(\'Save\',this,\'mouseout\')" onmouseover="ChangeButtonStyle(\'Save\',this,\'mouseover\')" value="OK" id="Save" name="Save" class="STbutton">'
	HtmlStr+='<input type="button" style="width: 75px; border-width: 1px; border-style: outset; color:#ffffff; background: none repeat scroll #7d7d7d;" onclick="ConfirmMsgNo(\''+mode+'\',\''+a+'\',\''+b+'\',\''+c+'\');" onmouseup="ChangeButtonStyle(\'Cancel\',this,\'mouseup\')" onmousedown="ChangeButtonStyle(\'Cancel\',this,\'mousedown\')" onmouseout="ChangeButtonStyle(\'Cancel\',this,\'mouseout\')" onmouseover="ChangeButtonStyle(\'Cancel\',this,\'mouseover\')" value="Cancel" id="Cancel" name="Cancel" class="SecSTbutton">'
	HtmlStr+='</td></tr>';
//	HtmlStr+='<input class="STbutton" type="button" value="OK" style="margin-right:20px;" onclick="ConfirmMsgYes(\''+mode+'\',\''+a+'\',\''+b+'\',\''+c+'\');"><input class="STbutton" type="button" value="Cancel"  onclick="ConfirmMsgNo(\''+mode+'\',\''+a+'\',\''+b+'\',\''+c+'\');"></td></tr>';
//	HtmlStr+='<tr><td><img src="images/_blank.gif" height="10"></td></tr>';
	HtmlStr+='</table>';
	HtmlStr+='</td></tr>';				
	HtmlStr+='</table>';
	HtmlStr+='</td>';
	HtmlStr+='</tr>';
    HtmlStr+='</table>';
	document.getElementById(confirmdiv).innerHTML=HtmlStr;
	//return 1;
}


function AlertMsg(alertTitle, msg, clickFunc)
{
	ConfirmCount++;
	
	var maskDivObj = document.getElementById ('maskAlertDiv');
	var popDivObj = document.getElementById ('AlertDiv');
	
	var numargs = arguments.length; 
	if(numargs ==2)
	{
		clickFunc ="";
	}
	
	maskDivObj.style.display = "block";
	popDivObj.style.display = "block";
	var HtmlStr="";
	HtmlStr+='<table width="100%" border="0" cellspacing="0" cellpadding="0" align="center" style="border:1px solid #fff;background-color:#f9f9f9;">'; //background:url(\'images/opacity67.png\');
	HtmlStr+='<tr >';
	HtmlStr+='<td width="100%" title="error" style="cursor:move;text-align:center;background-color:#7d7d7d;height:40px;">'; 
	HtmlStr+='<font style="font-size:18px;font-family:Arial Reg, Helvetica, sans-serif;color:#fff;">'+alertTitle+'</font></td> ';
	HtmlStr+='</tr>';
	
	HtmlStr+='<tr>';
	HtmlStr+='<td width="100%" style="padding:15px 20px 0px 20px;" >';

	HtmlStr+='<table bgcolor="#f9f9f9" cellspacing="0" cellpadding="0" width="100%">';
	
	HtmlStr+='<tr><td style="width:32px; padding: 0px 15px 15px 0px;">';
	HtmlStr+='<img src="images/icon_alert.gif" style="background:#f9f9f9;"></td>';
	HtmlStr+='<td align="left" style="valign:top; color:#303030; font-family:Arial Reg,Helvetica,sans-serif;font:13px; padding: 0px 0px 15px 0px;">'+msg+'</td>';
	HtmlStr+='</tr>';
	
	HtmlStr+='<tr><td colspan="2" ><hr style="color:#e2e2e2; margin: 0px; border-width: 1px;"></td></tr>';  
	
	HtmlStr+='<tr><td colspan="2" style="background:#f9f9f9;padding-top:15px;padding-bottom:15px;" valign="top" align="center">'; 
	HtmlStr+='<input onmouseup="ChangeButtonStyle(\'Save\',this,\'mouseup\')" onmousedown="ChangeButtonStyle(\'Save\',this,\'mousedown\')" onmouseout="ChangeButtonStyle(\'Save\',this,\'mouseout\')" onmouseover="ChangeButtonStyle(\'Save\',this,\'mouseover\')" value="OK" id="Save" name="Save" class="STbutton" type="button"   onclick="AlertMsgOnClickOK(\''+ clickFunc+'\');">';
	HtmlStr+='</td></tr>';
	
	HtmlStr+='</table>';
	
	HtmlStr+='</td>';
	HtmlStr+='</tr>';
    HtmlStr+='</table>';
	document.getElementById("AlertDiv").innerHTML = HtmlStr;
}

function AlertMsgOnClickOK(clickFunc)
{
	ConfirmCount--;
	
	var maskDivObj = document.getElementById ('maskAlertDiv');
	var popDivObj = document.getElementById ('AlertDiv');
	maskDivObj.style.display = "none";
	popDivObj.style.display = "none";
	
	eval(clickFunc);
	return;
}

function ConfirmMsgYes(n , a , b , c)
{
	if(ConfirmCount==0)
	{
		var confirmdiv="AlertDiv";
	}
	else
	{
		var confirmdiv="AlertDiv"+ConfirmCount.toString();
	}
	var maskDivObj = document.getElementById ('maskAlertDiv');
	var popDivObj = document.getElementById (confirmdiv);
	popDivObj.style.display = "none";
	ConfirmCount--;
	window.ConfirmMsgResult("yes", n ,a,b,c);
	if(ConfirmCount==-1)
	maskDivObj.style.display = "none";
	return;
}
function ConfirmMsgNo(n , a , b , c)
{
	if(ConfirmCount==0)
	{
		var confirmdiv="AlertDiv";
	}
	else
	{
		var confirmdiv="AlertDiv"+ConfirmCount.toString();
	}
	var maskDivObj = document.getElementById ('maskAlertDiv');
	var popDivObj = document.getElementById (confirmdiv);
	popDivObj.style.display = "none";
//	window.frames['contentFrame'].ConfirmMsgResult("no", n , a , b , c);
	ConfirmCount--;
	window.ConfirmMsgResult("no", n , a , b , c);
	if(ConfirmCount==-1)
	maskDivObj.style.display = "none";
	return;
}
function ChangeStyle(I,n)
{
	if(n==1)
	{
		I.src="/images/close_hover.gif";
	}
	else if(n==2)
	{
		I.src="/images/close.gif";
	}
}
function alertOk()
{
	var maskDivObj = document.getElementById ('maskAlertDiv');
	var popDivObj = document.getElementById ('AlertDiv');

	maskDivObj.style.display = "none";
	popDivObj.style.display = "none";
}

//=============== End of Common function ================*/

function closeWindow() 
{
	window.close();
	if (window.ActiveXObject);
	else window.location.href = "index.htm";
}
function style_display_on()
{
	if (window.ActiveXObject)		// For IE
		return "block";	
	else if (window.XMLHttpRequest)		// For Mozilla, Firefox 	
		return "table";
}

function MM_preloadImages() { //v3.0
   var d=document; 
   if(d.images)
   { 
   	if(!d.MM_p) 
   		d.MM_p=new Array();
   	var i,j=d.MM_p.length,a=MM_preloadImages.arguments; 
   	for(i=0; i<a.length; i++)
   	{
   		if (a[i].indexOf("#")!=0)
   		{ 
   			d.MM_p[j]=new Image; 
   			d.MM_p[j++].src=a[i];
   		}
   	}
   }	
}

function MM_swapImgRestore() { //v3.0
    var i,x,a=document.MM_sr; 
    for(i=0;a&&i<a.length&&(x=a[i])&&x.oSrc;i++) 
    	x.src=x.oSrc;
}

function MM_swapImage() { //v3.0
    var i,j=0,x,a=MM_swapImage.arguments; 
    document.MM_sr=new Array; 
    for(i=0;i<(a.length-2);i+=3)
    {
    	if ((x=MM_findObj(a[i]))!=null)
    	{
    		document.MM_sr[j++]=x; 
    		if(!x.oSrc) 
    			x.oSrc=x.src; 
    		x.src=a[i+2];
    	}
    }	
}

function MM_reloadPage(init) {  //reloads the window if Nav4 resized
  if (init==true) with (navigator) {if ((appName=="Netscape")&&(parseInt(appVersion)==4)) {
    document.MM_pgW=innerWidth; document.MM_pgH=innerHeight; onresize=MM_reloadPage; }}
  else if (innerWidth!=document.MM_pgW || innerHeight!=document.MM_pgH) location.reload();
}

MM_reloadPage(true);


function MM_findObj(n, d) { //v4.0
  var p,i,x;  if(!d) d=document; if((p=n.indexOf("?"))>0&&parent.frames.length) {
    d=parent.frames[n.substring(p+1)].document; n=n.substring(0,p);}
  if(!(x=d[n])&&d.all) x=d.all[n]; for (i=0;!x&&i<d.forms.length;i++) x=d.forms[i][n];
  for(i=0;!x&&d.layers&&i<d.layers.length;i++) x=MM_findObj(n,d.layers[i].document);
  if(!x && document.getElementById) x=document.getElementById(n); return x;
}

function MM_showHideLayers() { //v3.0

  var i,p,v,obj,args=MM_showHideLayers.arguments;

  for (i=0; i<(args.length-2); i+=3) if ((obj=MM_findObj(args[i]))!=null) { v=args[i+2];

    if (obj.style) { obj=obj.style; v=(v=='show')?'visible':(v='hide')?'hidden':v; }

    obj.visibility=v; }

}

function sTrim(I)
{

  var p=0;
  var q=0;
  var sL;
  var sTemp;

  sL=I.value.length;
  sTemp=I.value; 

  if (sL>0)
  {
      while(sTemp.charAt(p)==' ') p++;
	  while(sTemp.charAt((sL-1)-q)==' ') q++;
  }

  if (p==sL)
  	I.value="";
  else
	I.value=sTemp.substring(p,sL-q);
}

function notnull(Input, AlrtStr)	// check input string
{
    var str = Input.value;
	
    if (Input.value=="")
    {
    	 if (AlrtStr != "")
        	top.AlertMsg("Alert", AlrtStr);
        Input.value=Input.defaultValue;
        return;
    }

    str = replacechar(str, " ", "_");
    str = replacechar(str, ",", "");
    str = replacechar(str, ";", "");
    Input.value = str;	
}

function validatenum(Input, up, down, AlrtStr)	//check input number
{
    if (Input.value =="")
	return -1;	

    if (isNaN(Input.value) == true)
    {
        Input.value=Input.defaultValue;
	 return -1;	
    }
	
    var d;
    d=parseInt(Input.value,10);
    if (!(d<up && d>= down))
    {
        //alert(AlrtStr);
        
        top.AlertMsg("Alert", AlrtStr);
        Input.value=Input.defaultValue;	
        return -1;
    }
    Input.value=d;	
    return 1;	
}

function replacechar(Input, checkstr, replacestr)
{
	var p, q;
	var tempstring, tempname, tempspace="";
	
	if (Input != "")
	{
		tempstring = Input;
		p = tempstring.indexOf(checkstr);
		q = tempstring.length;
		
		if (p<0)
		{
			return Input;
		}
		else if (p==0)
			tempstring = tempstring.substring(p+1,q);
			
		while(1)
		{
			q = tempstring.length;
			p = tempstring.indexOf(checkstr);
			if (p >= 0)
			{
				tempname = tempstring;
				tempstring = tempname.substring(p+1,q);
				if (p < q)
				{
					if (p < q -1)
						tempspace += tempname.substring(0,p) + replacestr;
					else
						tempspace += tempname.substring(0,p);
				}	
			}
			else
				break;
		}
		return tempspace+tempstring;
	}
	else
		return tempspace;
}

function Check_User_Input(e)
{
	var keynum;
	
	if(window.event) // IE
		keynum = e.keyCode;
	else if(e.which) // Netscape/Firefox/Opera
		keynum = e.which;

	if (keynum < 47 || keynum > 57)
	{
		if (keynum != 8     //Backspace
		&& keynum != 13   //Enter 
		&& keynum != 46   //. Key
		&& keynum != 58)  //: Key
		{
			return false;
		}	
	}	

	return true;
}
function Check_Special_Words(word)
{

	var iChars = "'\"\\";
	var iChars2 = " ";
	
	for (var i = 0; i < word.value.length; i++)
	{
		if (iChars.indexOf(word.value.charAt(i)) != -1)
		{
			top.AlertMsg("Alert", aCheckName);
			return -1;
		}
	}
	return 1;
}

function Check_illegal_Words(word)
{
	var iChars = "'\"\\";
	
	for (var i = 0; i < word.value.length; i++)
	{
		if (iChars.indexOf(word.value.charAt(i)) != -1)
		{
			top.AlertMsg("Alert", aCheckName);
			return -1;
		}
	}
	return 0;
}

function chAll_Words()
{
	var objs1 = document.getElementsByTagName("input"); 
	if (objs1)
	for(var i=0;i < objs1.length; i++)  
	{  
		if( (objs1[i].type == "text" || objs1[i].type == "password" ) && objs1[i].readOnly == false && objs1[i].disabled == false) 
		{
			sTrim(objs1[i]);
			if(objs1[i].value.length>0)
			if(Check_Special_Words(objs1[i]) < 0)
			{						
				objs1[i].select();
				return;
			}
		} 
	}
	var objs2 = document.getElementsByTagName("option"); 
	if (objs2)
	for(var i=0;i < objs2.length; i++)  
	{  
		if(Check_illegal_Words(objs2[i]) <0)
		{
			objs2[i].selected=true;
			return;
		}
	}
	eval(Savelink);
}

function checkDate(I)
{
	if(I == null)
		return -1;
	if(I.value == "")
	{
		top.AlertMsg("Alert", "Please input Date!");
		I.value=I.defaultValue;
		return -1;
	}
	if(Check_Special_Words_j(I) < 0)
	{
		return -1;
	}
	
	var strIndex, strLen;	
	var nTmp;
	var tmpString;
	var rightString = I.value;
	var ts=new tmpWord(6);
	
	// have two '-'
	
	strLen = rightString.length;
	strIndex = rightString.indexOf(".");
	if(strIndex == 0 || strIndex == -1)
	{
		top.AlertMsg("Alert", "Please input date setting with correct format!");
		return -1;
	}
	ts[1] = rightString.substring(0, strIndex);
	tmpString = rightString.substring(strIndex +1, strLen);
	rightString = tmpString;
	
	strLen = rightString.length;
	strIndex = rightString.indexOf(".");
	if(strIndex == 0 || strIndex == -1)
	{
		top.AlertMsg("Alert", "Please input date setting with correct format!");
		return -1;
	}
	ts[2] = rightString.substring(0, strIndex);
	tmpString = rightString.substring(strIndex +1, strLen);
	rightString = tmpString;
	
	if(rightString =="")
	{
		top.AlertMsg("Alert", "Please input date setting with correct format!");
		return -1;
	}
	strLen = rightString.length;
	strIndex = rightString.indexOf(".");
	if(strIndex > 0)
	{
		top.AlertMsg("Alert", "Please input date setting with correct format!");
		return -1;
	}
	ts[3] = rightString;
	
	if (!(0<=ts[1] && ts[1]<=9999))
	{
		top.AlertMsg("Alert", aYearCheck);
		return -1;
	}
	
	if (!(ts[2]<=12 && ts[2]>=1))
	{
		top.AlertMsg("Alert", aMonthCheck);
	  return -1;
	}
	
	if (ts[2]==2)
	{
		if (ts[1]%400 == 0 ||(ts[1]%4 == 0 && ts[1]%100 == 0))//Freburary leap year 29 days
		{
			if (!(ts[3]<=29 && ts[3]>=1))
			{
				top.AlertMsg("Alert", aDay29Check);
				return -1;
			}
		}
		else
		{
			if (!(ts[3]<=28 && ts[3]>=1))
			{
				top.AlertMsg("Alert", aDay28Check);
				return -1;
			}
		}
	}	
	else if (ts[2]==4 || ts[2]==6 || ts[2]==9 || ts[2]==11)
	{
		if (!(ts[3]<=30 && ts[3]>=1))
		{
			top.AlertMsg("Alert", aDay30Check);
			return -1;
		}
	}
	else
	{
		if (!(ts[3]<=31 && ts[3]>=1))
		{
			top.AlertMsg("Alert", aDayCheck);
			return -1;
		}
	}
	
	if (ts[2].length==1) 
  {
  	nTmp=ts[2];
  	ts[2]="0"+nTmp;
  }
  if (ts[3].length==1) 
  {
  	nTmp=ts[3];
  	ts[3]="0"+nTmp;
  }
  
	I.value = ts[1] +"." +ts[2] +"." +ts[3];
	return 1;
}

function FilterDate(DateStr)
{
	var index = 0;
	var CompareChar=new Array("A", "D", "F", "J", "M", "N", "S", "O");
	for (var i=0; i<8;i++)
	{
		index = DateStr.indexOf(CompareChar[i],0);
		if (index < 0)
			continue;

		if (index > 0)
			return DateStr.substring(index, DateStr.length);
		else
			return DateStr;
	}
}

function checkDate2(I, year)
{
	if(I == null)
		return -1;
	if(I.value == "")
	{
		top.AlertMsg("Alert", "Please input Date!");
		I.value=I.defaultValue;
		return -1;
	}
	if(Check_Special_Words_j(I) < 0)
	{
		return -1;
	}
	
	var strIndex, strLen;	
	var nTmp;
	var tmpString;
	var rightString = I.value;
	var ts=new tmpWord(6);

	ts[1] = year; 
	strLen = rightString.length;
	strIndex = rightString.indexOf(".");
	if(strIndex == 0 || strIndex == -1)
	{
		top.AlertMsg("Alert", "Please input date setting with correct format!");
		return -1;
	}
	ts[2] = rightString.substring(0, strIndex);
	tmpString = rightString.substring(strIndex +1, strLen);
	rightString = tmpString;	
	
	if(rightString =="")
	{
		top.AlertMsg("Alert", "Please input date setting with correct format!");
		return -1;
	}
	strLen = rightString.length;
	strIndex = rightString.indexOf(".");
	if(strIndex > 0)
	{
		top.AlertMsg("Alert", "Please input date setting with correct format!");
		return -1;
	}
	ts[3] = rightString;
	/*
	if (!(0<=ts[1] && ts[1]<=9999))
	{
		alert(aYearCheck);
		return -1;
	}
	*/
	if (!(ts[2]<=12 && ts[2]>=1))
	{
		top.AlertMsg("Alert", aMonthCheck);
	  return -1;
	}
	
	if (ts[2]==2)
	{
		if (ts[1]%400 == 0 ||(ts[1]%4 == 0 && ts[1]%100 == 0))//Freburary leap year 29 days
		{
			if (!(ts[3]<=29 && ts[3]>=1))
			{
				top.AlertMsg("Alert", aDay29Check);
				return -1;
			}
		}
		else
		{
			if (!(ts[3]<=28 && ts[3]>=1))
			{
				top.AlertMsg("Alert", aDay28Check);
				return -1;
			}
		}
	}	
	else if (ts[2]==4 || ts[2]==6 || ts[2]==9 || ts[2]==11)
	{
		if (!(ts[3]<=30 && ts[3]>=1))
		{
			top.AlertMsg("Alert", aDay30Check);
			return -1;
		}
	}
	else
	{
		if (!(ts[3]<=31 && ts[3]>=1))
		{
			top.AlertMsg("Alert", aDayCheck);
			return -1;
		}
	}
	
	if (ts[2].length==1) 
  {
  	nTmp=ts[2];
  	ts[2]="0"+nTmp;
  }
  if (ts[3].length==1) 
  {
  	nTmp=ts[3];
  	ts[3]="0"+nTmp;
  }
  
	I.value = ts[2] +"." +ts[3];
	return 1;
}

function checkDateRange2(StartDate, EndDate, year)
{
	if(checkDate2(StartDate, year) < 0)
	{
		return -1;		
	}
	if(checkDate2(EndDate, year) < 0)
	{
		return -1;		
	}
	
	var strIndex, strLen;
	var tmpString;
	var rightString1 = StartDate.value;
	var rightString2 = EndDate.value;
	var ts1=new tmpWord(6);
	var ts2=new tmpWord(6);

	strLen = rightString1.length;
	strIndex = rightString1.indexOf(".");
	ts1[2] = rightString1.substring(0, strIndex);
	tmpString = rightString1.substring(strIndex +1, strLen);
	rightString1 = tmpString;	
	ts1[3] = rightString1;

	strLen = rightString2.length;
	strIndex = rightString2.indexOf(".");
	ts2[2] = rightString2.substring(0, strIndex);
	tmpString = rightString2.substring(strIndex +1, strLen);
	rightString2 = tmpString;
	ts2[3] = rightString2;
	
	var aStartLargerThanEnd ="Start Date must be earlier than End Date!";
	var year1=0, year2=0;
	var month1 = parseInt(ts1[2], 10);
	var month2 = parseInt(ts2[2], 10);
	var day1 = parseInt(ts1[3], 10);
	var day2 = parseInt(ts2[3], 10);
	/*purpose     : 0012654 author : Ben date : 2010-06-22*/
	/*description : Daylight Savings Time Dates should not be restricted
	if (month1 != 3 || (month1 == 3 && day1 > 14))
	{
		alert("The StartDate should be the 2nd Sunday in March!");
		return -1;
	}
	else if (month2 != 11 || (month2 == 11 && day1 > 7))
	{
		alert("The EndDate should be the 1st Sunday in November!");
		return -1;
	}
	*/	
	if (year1 >year2 )
	{
		top.AlertMsg("Alert", aStartLargerThanEnd);
		return -1;
	}
	else if(year1 == year2)
	{
		if(month1 >month2)
		{
			top.AlertMsg("Alert", aStartLargerThanEnd);
			return -1;
		}
		else if(month1 ==month2)
		{
			if(day1 >=day2)
			{
				top.AlertMsg("Alert", aStartLargerThanEnd);
				return -1;				
			}
		}
	}
	
	return 1;
}

function checkTime(I)
{
	if(I == null)
		return -1;
	if(I.value == "")
	{
		top.AlertMsg("Alert", "Please input Time!");
		I.value=I.defaultValue;
		return -1;
	}
	if(Check_Special_Words_jj(I) < 0)
	{
		return -1;
	}
	var strIndex, strLen;	
	var nTmp;
	var tmpString;
	var rightString = I.value;
	var ts=new tmpWord(6);
	var aFormatOfTime = "Please input time setting with correct format!";
	
	// have two '-'
	
	strLen = rightString.length;
	strIndex = rightString.indexOf(":");
	if(strIndex == 0 || strIndex == -1)
	{
		top.AlertMsg("Alert", aFormatOfTime);
		return -1;
	}
	ts[1] = rightString.substring(0, strIndex);
	tmpString = rightString.substring(strIndex +1, strLen);
	rightString = tmpString;
	
	strLen = rightString.length;
	strIndex = rightString.indexOf(":");
	if(strIndex == 0 || strIndex == -1)
	{
		top.AlertMsg("Alert", aFormatOfTime);
		return -1;
	}
	ts[2] = rightString.substring(0, strIndex);
	tmpString = rightString.substring(strIndex +1, strLen);
	rightString = tmpString;
	
	
	if(rightString =="")
	{
		top.AlertMsg("Alert", aFormatOfTime);
		return -1;
	}
	strLen = rightString.length;
	strIndex = rightString.indexOf(":");
	if(strIndex > 0)
	{
		top.AlertMsg("Alert", aFormatOfTime);
		return -1;
	}
	ts[3] = rightString;	
	
	if (!(ts[1]<=23 && ts[1]>=0))
	{
		top.AlertMsg("Alert", aHourCheck);
		return -1; 
	}
	if (!(ts[2]<=59 && ts[2]>=0))
	{
		top.AlertMsg("Alert", aMinuteCheck);
		return -1; 
	}
	if (!(ts[3]<=59 && ts[3]>=0))
	{
		top.AlertMsg("Alert", aSecondCheck);
		return -1; 
	}
	if (ts[1].length==1) 
	{
		nTmp=ts[1];
		ts[1]="0"+nTmp;
	}
	if (ts[2].length==1) 
	{
		nTmp=ts[2];
		ts[2]="0"+nTmp;
	}
	if (ts[3].length==1) 
	{
		nTmp=ts[3];
		ts[3]="0"+nTmp;
	}	
	
	I.value = ts[1] +":" +ts[2] +":" +ts[3];
	return 1;
}

/***************************************************
*** check time ***
***************************************************/
function Check_Special_Words_j(word)
{
	var iChars = "'\"\\";
	/*var iChars2 = " ";*/
	
	for (var i = 0; i < word.value.length; i++)
	{
		if (iChars.indexOf(word.value.charAt(i)) != -1)
		{
			top.AlertMsg("Alert", aCheckName);
			return -1;
		}
	}
	return 1;
}

function Check_Special_Words_jj(word)
{
	var iChars = "'\"\\";
	/*var iChars2 = " ";*/
	
	for (var i = 0; i < word.value.length; i++)
	{
		if (iChars.indexOf(word.value.charAt(i)) != -1)
		{
			top.AlertMsg("Alert", aCheckName);
			return -1;
		}
	}
	return 1;
}

function checkHourMinute(I)
{
	if(I == null)
		return -1;
	if(I.value == "")
	{
		top.AlertMsg("Alert", "Please input Time!");
		I.value=I.defaultValue;
		return -1;
	}
	if(Check_Special_Words_jj(I) < 0)
	{
		return -1;
	}
	var strIndex, strLen;	
	var nTmp;
	var tmpString;
	var rightString = I.value;
	var ts=new tmpWord(6);
	
	// have a ':'
	
	strLen = rightString.length;
	strIndex = rightString.indexOf(":");
	if(strIndex == 0 || strIndex == -1)
	{
		top.AlertMsg("Alert", "Please input time setting with correct format!");
		return -1;
	}
	ts[1] = rightString.substring(0, strIndex);
	tmpString = rightString.substring(strIndex +1, strLen);
	rightString = tmpString;
		
	if(rightString =="")
	{
		top.AlertMsg("Alert", "Please input time setting with correct format!");
		return -1;
	}
	strLen = rightString.length;
	strIndex = rightString.indexOf("-");
	if(strIndex > 0)
	{
		top.AlertMsg("Alert", "Please input time setting with correct format!");
		return -1;
	}
	ts[2] = rightString;	
	
	if (!(ts[1]<=23 && ts[1]>=0))
	  {
		top.AlertMsg("Alert", aHourCheck);
		return -1; 
	  }
	  if (!(ts[2]<=59 && ts[2]>=0))
	  {
		top.AlertMsg("Alert", aMinuteCheck);
		return -1; 
	  }
	  if (ts[1].length==1) 
	  {
	  	nTmp=ts[1];
	  	ts[1]="0"+nTmp;
	  }
	  if (ts[2].length==1) 
	  {
	  	nTmp=ts[2];
	  	ts[2]="0"+nTmp;
	  }
	
	I.value = ts[1] +":" +ts[2];
	return 1;
}

function checkHourMinuteRange(StartTime, EndTime)
{	
	if(checkHourMinute(StartTime) < 0)
	{
		return -1;		
	}
	if(checkHourMinute(EndTime) < 0)
	{
		return -1;		
	}
	
	var strIndex, strLen;
	var tmpString;
	var rightString1 = StartTime.value;
	var rightString2 = EndTime.value;
	var ts1=new tmpWord(6);
	var ts2=new tmpWord(6);
	
	strLen = rightString1.length;
	strIndex = rightString1.indexOf(":");
	ts1[1] = rightString1.substring(0, strIndex);
	tmpString = rightString1.substring(strIndex +1, strLen);
	rightString1 = tmpString;
	ts1[2] = rightString1;
	
	strLen = rightString2.length;
	strIndex = rightString2.indexOf(":");
	ts2[1] = rightString2.substring(0, strIndex);
	tmpString = rightString2.substring(strIndex +1, strLen);
	rightString2 = tmpString;
	ts2[2] = rightString2;
	
	var aStartLargerThanEnd="Start Time must be earlier than End Time!";
	var hour1 = parseInt(ts1[1], 10);
	var hour2 = parseInt(ts2[1], 10);
	var minute1 = parseInt(ts1[2], 10);
	var minute2 = parseInt(ts2[2], 10);
	if (hour1 >hour2)
	{
		top.AlertMsg("Alert", aStartLargerThanEnd);
		return -1;
	}
	else if(hour1 == hour2)
	{
		if(minute1 >= minute2)
		{
			top.AlertMsg("Alert", aStartLargerThanEnd);
			return -1;
		}
	}	
}

function hours_minutes_Check(I,mod_1,mod_2)
{
  var d;
  var single;
  if(I.value.length == 0)
   return ;
  var q = I.value.indexOf(':');
 
  if( q == -1 )
  {
  	alert("the time should be like:\'hh:mm\'");
  	I.value="";
  	I.select();
  	return	
  }
  else
  {
  	var hours= I.value.substring(0,q);
  	var minutes= I.value.substring(q+1,I.value.length);
  	
  	if(minutes.indexOf(':')!= -1 )
  	{
  		I.select();
  		I.value="";
  		return ;
  	}
  	 if (isNaN(hours) == true)
  	{
  		I.select();
  		I.value="";
  		return ;
  	}
  	 if (isNaN(minutes) == true)
  	{
  		I.select();
  		I.value="";
  		return ;
  	}
  
 
  if(mod_1 == "h")
  {
  	 d=parseInt(hours,10);
  	 if (!(d<24 && d>=0))
    {
      top.AlertMsg("Alert","The value (Hour) is out of range [0~23] !");
    I.select();
    I.value="";
	  return; 
    }
		hours=d;
    if (hours.toString().length==1) 
    {
      single=hours;
      hours="0"+single;
    }  	
  }
   if(mod_2 == "m")
  {
  	d=parseInt(minutes,10);
 
  	if (!(d<60 && d>=0))
    {
      top.AlertMsg("Alert","The value (Minute) is out of range [0~59] !");
    I.select();
    I.value="";
	  return;
    }
	  minutes = d;
	 
    if (minutes.toString().length==1) 
    {
      single=minutes;
      minutes="0"+single;
     
    }  	
  }
   I.value = hours.toString()+":"+ minutes.toString() ; 
   return ;
  } 
}
/**************************** Multi-selection operator ************************************/
function UpSel(s,up)
{
	var z;  
	var k;
	var i,j;
	if (s.length > 0)
	{
		tmp=new tmpWord(s.length);
		tmpChanged=new tmpWord(s.length); 
		opvtmp=new tmpWord(s.length);
		opvtmpChanged=new tmpWord(s.length);
		for (i=0; i < s.length; i++)
		{
			tmp[i+1]=s.options[i].text; 
			opvtmp[i+1]=s.options[i].value;	  
		}	
		if(up==1)
		{
			for (i=0; i < s.length; i++)
			{
				if (s.options[i].selected==true)
				{ 
					tmp[0]=s.options[i-up].text;
					opvtmp[0]=s.options[i-up].value;	
					s.options[i-up].value=s.options[i].value;		  		
					s.options[i-up].text=s.options[i].text;
					s.options[i].value=opvtmp[0];
					s.options[i].text=tmp[0];
					s.options[i].selected=false;
					s.options[i-up].selected=true;		
				}
			}
		}
		if(up==-1)
		{
			for (i=s.length-1; i > -1; i--)
			{
				if (s.options[i].selected==true)
				{ 
					tmp[0]=s.options[i-up].text;
					opvtmp[0]=s.options[i-up].value;	
					s.options[i-up].value=s.options[i].value;		  		
					s.options[i-up].text=s.options[i].text;
					s.options[i].value=opvtmp[0];
					s.options[i].text=tmp[0];
					s.options[i].selected=false;
					s.options[i-up].selected=true;		
				}
			}
		}
	}
  	SetButtonStatus("upRate", "disabled");
  	SetButtonStatus("downRate", "disabled");
	if(s.options[0].selected!=true)
  		SetButtonStatus("upRate", "");
	if(s.options[s.length-1].selected!=true)	
  		SetButtonStatus("downRate", "");
}	
function delSel(s,I)
{
	var z;  
	var k;

	if (s.length > 0)
	{
		tmp=new tmpWord(s.length);
		tmpChanged=new tmpWord(s.length); 
		opvtmp=new tmpWord(s.length);
		opvtmpChanged=new tmpWord(s.length); 

		for (var i=0; i < s.length; i++)
		{
			tmp[i+1]=s.options[i].text;
			opvtmp[i+1]=s.options[i].value;
		}	

		for (var i=0; i < s.length; i++)
		{
			if (s.options[i].selected==true)
			{ 
				s.options[i].text="";
				s.options[i].value="";
				tmp[i+1]=" ";
				opvtmp[i+1]=" ";		
				s.options[i].selected=false;	      
			}
		}
		k=1;
		z=0;
		for (var j=1; j<=s.length; j++) 
		{ 
			if (tmp[j]!=" ") 
			{
				tmpChanged[k]=tmp[j];
				opvtmpChanged[k]=opvtmp[j];
				k++;
			}
			else
			{
				z++;
			}
		}
		for (var i=0; i < s.length-z; i++)
		{
			  s.options[i].text=tmpChanged[i+1]; 
			  s.options[i].value=opvtmpChanged[i+1];  	 
		}
		s.length-=z;
	}
	clearContent(s.form,I); 
}

function exPosion(s)
{
	if (s.length > 0)
	{
		tmp=new tmpWord(s.length);
		tmpChanged=new tmpWord(s.length); 
		opvtmp=new tmpWord(s.length);
		opvtmpChanged=new tmpWord(s.length);
		 	
		for (var i=0; i < s.length; i++)
		{
			tmp[i+1]=s.options[i].text;
			opvtmp[i+1]=s.options[i].value;
		}	
			
		for (var i=0; i < s.length; i++)
		{
			s.options[i].text=tmp[s.length-i];
			s.options[i].value=opvtmp[s.length-i];
		}

		for (var i=0; i < s.length; i++)
		{
			tmp[i+1]=" ";
			opvtmp[i+1]=" ";	
		}
	}
}
function selAll(s)
{
	if (s.length>0)
	{
		//exPosion(s);
		for (var i=0; i < s.length; i++)
		s.options[i].selected=true;
	}
}

function tmpWord(n)
{
	this.length=n;
	for (var i=1; i<=n; i++)
		this[i]=0;
	return this;
}

/*********open table*******************/
var wservice_window=null;
var wstatus_window=null;
var wsetting_window=null;
var wabout_window=null;
var whelp_window=null;

function openTable(n)
{	
    switch (n)
    {

	case 'about.htm':
   	        if (wabout_window!=null) closeTable(wabout_window);
		 {
		 	if (ModelName == "LRT214" )
				wabout_window=window.open('214/about.htm', '','menubar=no, width=480, height=425');
			else if (ModelName == "LRT224" )
				wabout_window=window.open('224/about.htm', '','menubar=no, width=480, height=425');
			else
	    			wabout_window=window.open('082/about.htm', '','menubar=no, width=480, height=425');		
   	        }		
		 break;
	case 'Dhcp_table1.htm':
   	        if (wsetting_window!=null) closeTable(wsetting_window);
	    		wsetting_window=window.open(n, '','location=yes, resizable, menubar=no, scrollbars, width=680, height=600');		
		break;		 
	case 'sys_log.htm':
	case 'outgoing_log.htm':
	case 'incoming_log.htm':
	case 'Routing_table.htm':
	case 'UPnP_table.htm':
   	        if (wstatus_window!=null) closeTable(wstatus_window);
	    		wstatus_window=window.open(n, '','location=yes, resizable, menubar=no, scrollbars, width=820');		
		break;
			
    	default:
		if (wsetting_window!=null) closeTable(wsetting_window);
			wsetting_window=window.open(n, '','location=yes, resizable, menubar=no, scrollbars, width=820');		
	    	break;
    }	
}
function callHelp()
{	
	if (!helplink)
		return;

//        if (whelp_window!=null) closeTable(whelp_window);
	 {
/*	 	
	 	if (TotalPortNumber == 16)
			whelp_window=window.open('016/'+helplink, '','location=yes, menubar=no, scrollbars=yes , width=640,height=600');
		else if (TotalPortNumber == 6)
			whelp_window=window.open('042/'+helplink, '','location=yes, menubar=no, scrollbars=yes , width=640,height=600');
		else
			whelp_window=window.open('082/'+helplink, '','location=yes, menubar=no, scrollbars=yes , width=640,height=600');
*/
		//whelp_window=window.open('help/help_index.htm?url='+helplink, '','location=yes, menubar=no, scrollbars=yes , width=640,height=600');
		whelp_window=window.open('lrt2x4_help/index.html?'+helplink, 'help','location=yes, menubar=no, scrollbars=yes ,  resizable=yes,width=840,height=600');
		whelp_window.focus();
        }	
}
function closeTable(wThis)
{
    if (wThis!=null)
    {
      wThis.close();
      wThis=null;
    }
}
function chLeave()
{
	 if (wabout_window!=null) closeTable(wabout_window);
	 //if (whelp_window!=null) closeTable(whelp_window);
}

/* 2004/08/18 Eric --> Network Check */
function IpToArray(V, n) // IP (or Mask) value [ex: 192.168.1.1], want to get IP index (1~4)
{
	var ip1,ip2,ip3,ip4;
	var p,q,rightString,tmpString;

	rightString=V;
/**/
	q=rightString.length;
	p=rightString.indexOf(".");
	ip1=rightString.substring(0,p); 
	tmpString=rightString;
	rightString=tmpString.substring(p+1,q); 
/*.*/

	q=rightString.length;
	p=rightString.indexOf(".");
	ip2=rightString.substring(0,p); 
	tmpString=rightString;
	rightString=tmpString.substring(p+1,q); 
/*.*/

	q=rightString.length;
	p=rightString.indexOf(".");
	ip3=rightString.substring(0,p); 
	tmpString=rightString;

/*.*/
	ip4=tmpString.substring(p+1,q); 

/*.*/
	if (n=="1") return ip1;
	if (n=="2") return ip2;
	if (n=="3") return ip3;
	if (n=="4") return ip4;	// get IP index (1~4)
}
function NetworkToArray(ip, mask, n) // IP value [ex: 192.168.1.1], Mask value [ex: 255.255.255.0], want to get NETWORK ID index (1~4)
{
    var ip1,ip2,ip3,ip4;
	var mask1,mask2,mask3,mask4;
    var network1,network2,network3,network4;
	
	ip1=IpToArray(ip, '1');
	ip2=IpToArray(ip, '2');
	ip3=IpToArray(ip, '3');
	ip4=IpToArray(ip, '4');
	
	mask1=IpToArray(mask, '1');
	mask2=IpToArray(mask, '2');
	mask3=IpToArray(mask, '3');
	mask4=IpToArray(mask, '4');
	
	network1 = ip1 & mask1; 
	network2 = ip2 & mask2;
	network3 = ip3 & mask3; 
	network4 = ip4 & mask4; 
	
	if (n=="1") return network1;
	if (n=="2") return network2;
	if (n=="3") return network3;
	if (n=="4") return network4; // get NETWORK ID index (1~4)
}
// 2004/11/30 Eric -->
function BroadcastToArray(ip, mask, n) // IP value [ex: 192.168.1.1], Mask value [ex: 255.255.255.0], want to get BROADCAST ID index (1~4)
{
    var ip1,ip2,ip3,ip4;
	var mask1,mask2,mask3,mask4;
    var network1,network2,network3,network4;
	
	ip1=IpToArray(ip, '1');
	ip2=IpToArray(ip, '2');
	ip3=IpToArray(ip, '3');
	ip4=IpToArray(ip, '4');
	
	mask1=IpToArray(mask, '1');
	mask2=IpToArray(mask, '2');
	mask3=IpToArray(mask, '3');
	mask4=IpToArray(mask, '4');
	
	network1 = ip1 | (255-mask1); 
	network2 = ip2 | (255-mask2);
	network3 = ip3 | (255-mask3); 
	network4 = ip4 | (255-mask4); 
	
	if (n=="1") return network1;
	if (n=="2") return network2;
	if (n=="3") return network3;
	if (n=="4") return network4; // get NETWORK ID index (1~4)
}
var aNetworkRangeCheck="IP Address is out of range "; 
function NetworkRangeCheck(I, ip, mask, n)
{
  var d;
  var netip_start, netip_end;

  d=parseInt(I.value,10);
  netip_start=eval(NetworkToArray(ip, mask, n));
  netip_end=eval(BroadcastToArray(ip, mask, n));
 
  if (n=="4")
  {
      	netip_start++;
      	netip_end--;
  }
  //if DMZ_host ip=0;don't  alert message
  if(!(I.name=="dmzAddr4"&&I.value==0))
  if (!(d<=netip_end && d>=netip_start)) 
  {
    var alertmsg = aNetworkRangeCheck+"["+netip_start+"~"+netip_end+"] !";
	top.AlertMsg("Alert", alertmsg);
    I.value=I.defaultValue;
    return;   
  }
  I.value=d;  
  return;	
	
}
function NetworkRangeCheck1(I, ip, mask, n)
{
  var d;
  var netip_start, netip_end;

  d=parseInt(I.value,10);
  netip_start=eval(NetworkToArray(ip, mask, n));
  netip_end=eval(BroadcastToArray(ip, mask, n));
 
  if (n=="4")
  {
      	netip_start++;
      	netip_end--;
  }

  //if DMZ_host ip=0;don't  alert message
  if(!(I.name=="dmzAddr4"&&I.value==0))
  if (!(d<=netip_end && d>=netip_start)) 
  {
    var alertmsg = aNetworkRangeCheck+"["+netip_start+"~"+netip_end+"] !";
	top.AlertMsg("Alert", alertmsg);
//    I.value=I.defaultValue;
    return -1;   
  }
  I.value=d;  
  return 1;	
	
}

function IPSaver(CheckIP,ip1,ip2,ip3,ip4)
{
	var ts=new tmpWord(4);
	var rightString;
	var tmpString;
	var i,p,q;
	tmpString=CheckIP.value;
	for(i=1;i<5;i++)
	{
		q=tmpString.length;
		p=tmpString.indexOf(".");
		if(i==4)
			ts[i]=tmpString;
		else
		{
			ts[i]=tmpString.substring(0,p);
			rightString=tmpString;
			tmpString=rightString.substring(p+1,q);
		}
	}
	ip1.value=ts[1];
	ip2.value=ts[2];
	ip3.value=ts[3];
	ip4.value=ts[4];

	return;
}

function IPCheck(CheckIP, Min, Max, AllowScope, ForbiddenIP, NoAlert, IPName)
{
	var i,j,p,q;
  	var tmpString;
	var rightString;
	var ts=new tmpWord(4);
	var ts2=new tmpWord(4);
	var ts3=new Array(254,252,248,240,224,192,128);
	var AlertName = "This IP";

	if (IPName)
		AlertName = IPName;

	if (!Max || isNaN(Max) == true)
		Max = 254;
	
	if (!Min || isNaN(Min) == true)
		Min = 0;	
	
	tmpString=CheckIP.value;
	for(i=1;i<5;i++)
	{
		q=tmpString.length;
		p=tmpString.indexOf(".");
		if(i==4)
		{
			ts[4]=tmpString.substring(0,p);
			rightString = "";
			rightString = tmpString.substring(p,q);
			if (ts[4] == "")
			{
				ts[4]=tmpString;
				rightString = "";
			}

			ts[4] = replacechar(ts[4], " ", "");
			if (isNaN(ts[4]) == true || ts[4].length ==0 )
			{
				if (NoAlert);
				else top.AlertMsg("Alert", AlertName+"\'s format is illegal! ");
				return -1;
			}
			
			if(ts[4] < Min || ts[4] > Max)
			{
				if (NoAlert);
				else top.AlertMsg("Alert", AlertName+'\'s last value is out of range! ['+Min+'~'+Max+'] ');
				return -1;
			}

			if (rightString.length > 0)
			{
				if (NoAlert);
				else top.AlertMsg("Alert", 'Please input '+AlertName+' with correct format!');
				return -1;
			}
		}
		else if(p<=0)
		{
			if (CheckIP.value == "") return 0;
			else
			{
				if (NoAlert);
				else top.AlertMsg("Alert", 'Please input '+AlertName+' with correct format!');
			}	
			return -1;
		}
		else
		{
			ts[i]=tmpString.substring(0,p);
			rightString=tmpString;
			tmpString=rightString.substring(p+1,q);
			ts[i] = replacechar(ts[i], " ", "");
		}
		
		if(isNaN(ts[i]) == true || ts[i]<0||ts[i]>255)
		{
			if (NoAlert);
			else top.AlertMsg("Alert", AlertName+'\'s format is illegal! ');
			return -1;
		}
		if(AlertName == 'Subnet Mask')
		{
			if(ts[1] == 0)
			{
				if (NoAlert);
				else top.AlertMsg("Alert", AlertName+'\'s format is illegal! ');
				return -1;
			}
			if((ts[1] != 0) && (ts[1] != 255))
			{
				for(j=0;j<7;j++)
				{
					if(ts[1] == ts3[j])
					{
						break;
					}
					if(j==6)
					{
						if (NoAlert);
						else top.AlertMsg("Alert", AlertName+'\'s format is illegal! ');
						return -1;	
					}
				}
				if((ts[2] != 0) || (ts[3] != 0) || (ts[4] != 0))
				{
					if (NoAlert);
					else top.AlertMsg("Alert", AlertName+'\'s format is illegal! ');
					return -1;
				}
			}
			if(ts[1] == 255)
			{
				if((ts[2] != 0) && (ts[2] != 255))
				{
					for(j=0;j<7;j++)
					{
						if(ts[2] == ts3[j])
						{
							break;
						}
						if(j==6)
						{
							if (NoAlert);
							else top.AlertMsg("Alert", AlertName+'\'s format is illegal! ');
							return -1;	
						}
					}
					if((ts[3] != 0) || (ts[4] != 0))
					{
						if (NoAlert);
						else top.AlertMsg("Alert", AlertName+'\'s format is illegal! ');
						return -1;
					}
				}
				if(ts[2] == 255)
				{
					if((ts[3] != 0) && (ts[3] != 255))
					{
						for(j=0;j<7;j++)
						{
							if(ts[3] == ts3[j])
							{
								break;
							}
							if(j==6)
							{
								if (NoAlert);
								else top.AlertMsg("Alert", AlertName+'\'s format is illegal! ');
								return -1;	
							}	
						}
						if(ts[4] != 0)
						{
							if (NoAlert);
							else top.AlertMsg("Alert", AlertName+'\'s format is illegal! ');
							return -1;
						}
					}
					if(ts[3] == 255)
					{
						if((ts[4] != 0) && (ts[4] != 255))
						{
							for(j=0;j<7;j++)
							{
								if(ts[4] == ts3[j])
								{
									break;
								}
								if(j==6)
								{
									if (NoAlert);
									else top.AlertMsg("Alert", AlertName+'\'s format is illegal! ');
									return -1;	
								}
							}
						}
					}
					if((ts[3] == 0) && (ts[4] != 0))
					{
						if (NoAlert);
						else top.AlertMsg("Alert", AlertName+'\'s format is illegal! ');
						return -1;	
					}
				}
				if((ts[2] == 0) && ((ts[3] != 0) || (ts[4] != 0)))
				{
					if (NoAlert);
					else top.AlertMsg("Alert", AlertName+'\'s format is illegal! ');
					return -1;	
				}
			}
		}
	}	
	if (AllowScope)
	{
		tmpString=AllowScope.value;
		for(i=1;i<5;i++)
		{
			q=tmpString.length;
			p=tmpString.indexOf(".");
			if(i==4)
			{
				ts2[i]=tmpString;
			}
			else
			{
				ts2[i]=tmpString.substring(0,p);
				rightString=tmpString;
				tmpString=rightString.substring(p+1,q);
			}
		}
		for(i=1;i<4;i++)
		{
			if(ts[i]!=ts2[i])
			{
				if (NoAlert);
				else top.AlertMsg("Alert", AlertName+'\'s value should be under '+ts2[1]+'.'+ts2[2]+'.'+ts2[3]+'.'+ Min +' ~ '+Max+'! ');
				return -2;
			}
		}
	}
	
	if (ForbiddenIP)
	{
		tmpString=ForbiddenIP.value;
		for(i=1;i<5;i++)
		{
			q=tmpString.length;
			p=tmpString.indexOf(".");
			if(i==4)
			{
				ts2[i]=tmpString;
			}
			else
			{
				ts2[i]=tmpString.substring(0,p);
				rightString=tmpString;
				tmpString=rightString.substring(p+1,q);
			}
		}
		if(ts[1] == ts2[1] && ts[2] == ts2[2] && ts[3] == ts2[3] && ts[4] == ts2[4])
		{
			if (NoAlert);
			else top.AlertMsg("Alert", AlertName+' can\'t be '+ts2[1]+'.'+ts2[2]+'.'+ts2[3]+'.'+ts2[4]+'! ');
			return -3;	
		}		
	}	
	CheckIP.value = ts[1] + "." + ts[2] + "." + ts[3] + "." + ts[4];	
	return 0;
}

function PortCheck(I)
{
	return validatenum(I, 65536, 0, aPort0Check);
}

function IPRangeCheck(StartIP, EndIP, AllowScope, ForbiddenIP, InputColume, NoAlert) // Only Support Class C
{
	var i,p,q;
  	var tmpString;
	var rightString;
	var ts=new tmpWord(4);
	var ts2=new tmpWord(4);
	var ts3=new tmpWord(4);	
	var StartIPisRight = 0;

	if (InputColume);
	else if (IPCheck(StartIP, 1, 254, AllowScope, 0, 0, "Start IP") <0) return -1;

	if (StartIP.value != "")
	{
		StartIPisRight = 1;
		tmpString=StartIP.value;
		for(i=1;i<5;i++)
		{
			q=tmpString.length;
			p=tmpString.indexOf(".");

			if(i==4)
				ts[i]=tmpString;
			else
			{
				ts[i]=tmpString.substring(0,p);
				rightString=tmpString;
				tmpString=rightString.substring(p+1,q);
			}
		}
		if (EndIP.value=="")
			EndIP.value= ts[1] + '.' + ts[2] + '.' + ts[3] + '.' + ts[4];
	}
	if (StartIPisRight)
		if (IPCheck(EndIP, 1, 254, StartIP, 0, 0, "End IP") <0) return -1;
	else
		if (IPCheck(EndIP, 1, 254, AllowScope, 0, 0, "End IP") <0) return -1;

	tmpString=EndIP.value;
	for(i=1;i<5;i++)
	{
		q=tmpString.length;
		p=tmpString.indexOf(".");
		if(i==4)
			ts2[i]=tmpString;
		else
		{
			ts2[i]=tmpString.substring(0,p);
			rightString=tmpString;
			tmpString=rightString.substring(p+1,q);
		}		
	}
	if(StartIPisRight == 1 && parseInt(ts2[4],10) < parseInt(ts[4],10))
	{
		if (NoAlert);
		else top.AlertMsg("Alert", 'The End IP should be bigger than Start IP! ');
		return -2;
	}		
	if (ForbiddenIP)
	{
		tmpString=ForbiddenIP.value;
		for(i=1;i<5;i++)
		{
			q=tmpString.length;
			p=tmpString.indexOf(".");
			if(i==4)
			{
				ts3[i]=tmpString;
			}
			else
			{
				ts3[i]=tmpString.substring(0,p);
				rightString=tmpString;
				tmpString=rightString.substring(p+1,q);
			}
		}	
	
		
		if(parseInt(ts3[4]) >= parseInt(ts[4]) && parseInt(ts3[4]) <= parseInt(ts2[4]))
		{
			if (NoAlert);
			else if (StartIP.value == "") top.AlertMsg("Alert", aIPAddressStart);
			else 
			{
				var alertmsg = 'This IP Range can\'t contain '+ts3[1]+'.'+ts3[2]+'.'+ts[3]+'.'+ts3[4]+'! ';
				top.AlertMsg("Alert", alertmsg);
			}
			return -3;	
		}		
	}

	return 0;	
}

function MACCheck(CheckMAC, MACName) 
{    
	var tmp=CheckMAC.value.split(":");
	var tmp2=CheckMAC.value.split("-");
	var mac_string ="";
	var AlertName = "This MAC";	

	if (MACName)
		AlertName = AlertName;

	if(tmp.length == 6)
		mac_string=tmp[0]+tmp[1]+tmp[2]+tmp[3]+tmp[4]+tmp[5];
	else if(tmp2.length == 6)
		mac_string=tmp2[0]+tmp2[1]+tmp2[2]+tmp2[3]+tmp2[4]+tmp2[5];
	else
		mac_string=CheckMAC.value;

	mac_string=mac_string.toUpperCase();
	if(mac_string.length != 12)
	{
		if (CheckMAC.value == "") return 0;
		else
		{
			var alertmsg = 'Please input '+AlertName+' with correct format!';
			top.AlertMsg("Alert", alertmsg);
		}
		return -1;                    
	}

	for (var i=0; i < mac_string.length; i++)
	{
		if( (mac_string.charAt(i) < "A" || mac_string.charAt(i) > "F" ) )
		if( (mac_string.charAt(i) < "0" || mac_string.charAt(i) > "9"))
		{
			var alertmsg=AlertName +'\'s format is illegal! ';
			top.AlertMsg("Alert", alertmsg);
			return -2;                  
		}
	}
	return 0;
}

function filterMAC(MAC) 
{
	var type=":";
	var tmp=MAC.split(":");
	var tmp2=MAC.split("-");
	var mac_string;

	if(tmp.length == 6)
		mac_string=tmp[0]+tmp[1]+tmp[2]+tmp[3]+tmp[4]+tmp[5];
	else if(tmp2.length == 6)
		mac_string=tmp2[0]+tmp2[1]+tmp2[2]+tmp2[3]+tmp2[4]+tmp2[5];
	else
		mac_string=MAC;		
	if(mac_string.length == 12)
	{
		mac_string=mac_string.charAt(0)+mac_string.charAt(1)+type+mac_string.charAt(2)+mac_string.charAt(3)+type+
				   mac_string.charAt(4)+mac_string.charAt(5)+type+mac_string.charAt(6)+mac_string.charAt(7)+type+
				   mac_string.charAt(8)+mac_string.charAt(9)+type+mac_string.charAt(10)+mac_string.charAt(11);
		mac_string=mac_string.toUpperCase();                   
	}
	return mac_string;     
}

function CheckIPv6(IP,type) 
{
    var v6rule,v6rule2002;

	v6ruleRegularZip=/^((([0-9a-f]{1,4}:){7}[0-9a-f]{1,4})|(([0-9a-f]{1,4}:){6}:[0-9a-f]{1,4})|(([0-9a-f]{1,4}:){5}:([0-9a-f]{1,4}:)?[0-9a-f]{1,4})|(([0-9a-f]{1,4}:){4}:([0-9a-f]{1,4}:){0,2}[0-9a-f]{1,4})|(([0-9a-f]{1,4}:){3}:([0-9a-f]{1,4}:){0,3}[0-9a-f]{1,4})|(([0-9a-f]{1,4}:){2}:([0-9a-f]{1,4}:){0,4}[0-9a-f]{1,4})|([0-9a-f]{1,4}::([0-9a-f]{1,4}:){0,5}[0-9a-f]{1,4})|(::([0-9a-f]{1,4}:){0,6}[0-9a-f]{1,4})|(([0-9a-f]{1,4}:){1,7}:))$/;
	v6Begin=/^(2002)/; //fc00|fe80

	if (type=="DNS"){
		if (IP.value=="::1"){
			top.AlertMsg("Alert", "\"::1\" cannot be IPv6 address.");
			return false;
		}
		else if (IP.value=="")
		{
			top.AlertMsg("Alert", "Please input IPv6 Address !");
		}
		else if (IP.value!="::")
		{
			if (!v6ruleRegularZip.test(IP.value) || v6Begin.test(IP.value)){
				top.AlertMsg("Alert", "Please input IPv6 Address with correct format!");
				return false;
			}else return true;
		}
		return true;
	}
	else
	{
		if (IP.value=="::1" || IP.value=="::"){
			top.AlertMsg("Alert", "\"::\" or \"::1\" cannot be IPv6 address.");
			return false;
		}

		if (!v6ruleRegularZip.test(IP.value) || v6Begin.test(IP.value)){
			top.AlertMsg("Alert", "Please input IPv6 Address with correct format!");
			return false;
		}else return true;
	}

	/*if (type=="local"){
		if (!v6ruleRegularZip.test(IP.value)|| !v6Begin.test(IP.value)){
			alert("Please input IPv6 Address with correct format!");
			return false;
		}else return true;
	}
	else if (type=="global")
	{
		if (!v6ruleRegularZip.test(IP.value)){
			alert("Please input IPv6 Address with correct format!");
			return false;
		}else return true;
	}*/

}
function CheckIPv6Prefix(Prefix) 
{
	if (Prefix.value>128 || Prefix.value<0)
	{
        top.AlertMsg("Alert", "Please set the prefix length within the range of 0-128.");
		return false;
	}else return true;

}

function CHeckIPv6_smallLarge(small_ip, large_ip, prefix){

    var hex_prefix_small1,hex_prefix_small2,hex_prefix_small3,hex_prefix_small4;
	var hex_prefix_large1,hex_prefix_large2,hex_prefix_large3,hex_prefix_large;
	var i;

	//small_ip   
	i=hex_to_ten(small_ip);	
	hex_prefix_small1=parseInt(i/8);
	hex_prefix_small2=parseInt((i-hex_prefix_small1*8)/4);
	hex_prefix_small3=parseInt((i-hex_prefix_small1*8-hex_prefix_small2*4)/2);
	hex_prefix_small4=parseInt((i-hex_prefix_small1*8-hex_prefix_small2*4-hex_prefix_small3*2));

	//large_ip  
	i=hex_to_ten(large_ip);
	hex_prefix_large1=parseInt(i/8);
	hex_prefix_large2=parseInt((i-hex_prefix_large1*8)/4);
	hex_prefix_large3=parseInt((i-hex_prefix_large1*8-hex_prefix_large2*4)/2);
	hex_prefix_large4=(i-hex_prefix_large1*8-hex_prefix_large2*4-hex_prefix_large3*2);

	switch(prefix){		
		case 3:
			if (hex_prefix_small3!=hex_prefix_large3)
				return false;
		case 2:
			if (hex_prefix_small2!=hex_prefix_large2)
				return false;
		case 1:
			if (hex_prefix_small1!=hex_prefix_large1)
				return false;
		default:
			;
	}

	return true;

}

function hex_to_ten(char_ten){
	var hex_value;

	switch (char_ten)
	{
		case 'a':
			hex_value=10;
			break;
		case 'b':
			hex_value=11;
			break;
		case 'c':
			hex_value=12;
			break;
		case 'd':
			hex_value=13;
			break;
		case 'e':
			hex_value=14;
			break;
		case 'f':
			hex_value=15;
			break;
		default:
			hex_value=char_ten;
	}

	return hex_value;
}

function IPRangeCheck_v6(ip_start, ip_end, v6LanIp, prefix, type)
{	
	var tmp_start=ip_start.value.split(":");
	var tmp_end=ip_end.value.split(":");
	var tmp_lan=v6LanIp.value.split(":");
	var tmp;
	var samefrefix=true;

	if (type=="start")
	{
		if(!CheckIPv6(ip_start,"local"))
		return;
	}
	else if (type=="end")
	{
		if(!CheckIPv6(ip_end,"local"))
		return;
	}
	else
	{	
		if(!CheckIPv6(ip_start,"global"))
			return;
		
		if (type!="zero")
		{
			if(!CheckIPv6(ip_end,"global"))
				return;
		}
	}	

	//ip_start
	for (var i = 0; i < 8; i++)
	{
		// special case x:x:x:x:x:x:x:: would be considered as x:x:x:x:x:x:x:0000
		// special case ::x:x:x:x would be considered as 0000::x:x:x:x
		if (i != 0 && i != 7 && tmp_start[i].length==0) // find ::
		{
			tmp = 8 - (tmp_start.length - 1);
			for (var j = tmp_start.length - 1 - i - 1; j >= 0; j--)
			{
				tmp_start[i+tmp+j] = tmp_start[(i+1)+j];
			}
			for (var j = 0; j < tmp; j++)
			{
				tmp_start[i+j] = "0000";
			}
		}
		switch (tmp_start[i].length)
		{
			case 0:
				tmp_start[i]="0000";
				break;
			case 1:
				tmp_start[i]="000"+tmp_start[i];
				break;
			case 2:
				tmp_start[i]="00"+tmp_start[i];
				break;
			case 3:
				tmp_start[i]="0"+tmp_start[i];
				break;
			default:
				break;
		}
	}
	//alert(tmp_start[0]+" "+tmp_start[1]+" "+tmp_start[2]+" "+tmp_start[3]+" "+tmp_start[4]+" "+tmp_start[5]+" "+tmp_start[6]+" "+tmp_start[7]);

	if (type=="zero"){
		
		for (var j=0; j<32 ; j++ )
		{
			if(tmp_start[parseInt(j/4)].charAt(j%4) != 0)
				return 0;
		}
		return 1;
	}

	//ip_end
	for (var i = 0; i < 8; i++)
	{
		// special case x:x:x:x:x:x:x:: would be considered as x:x:x:x:x:x:x:0000
		// special case ::x:x:x:x would be considered as 0000::x:x:x:x
		if (i != 0 && i != 7 && tmp_end[i].length==0) // find ::
		{
			tmp = 8 - (tmp_end.length - 1);
			for (var j = tmp_end.length - 1 - i - 1; j >= 0; j--)
			{
				tmp_end[i+tmp+j] = tmp_end[(i+1)+j];
			}
			for (var j = 0; j < tmp; j++)
			{
				tmp_end[i+j] = "0000";
			}
		}
		switch (tmp_end[i].length)
		{
			case 0:
				tmp_end[i]="0000";
				break;
			case 1:
				tmp_end[i]="000"+tmp_end[i];
				break;
			case 2:
				tmp_end[i]="00"+tmp_end[i];
				break;
			case 3:
				tmp_end[i]="0"+tmp_end[i];
				break;
			default:
				break;
		}
	}
	//alert(tmp_end[0]+" "+tmp_end[1]+" "+tmp_end[2]+" "+tmp_end[3]+" "+tmp_end[4]+" "+tmp_end[5]+" "+tmp_end[6]+" "+tmp_end[7]);

	if (type=="sort")
	{
		for (var j = 0; j < 32; j++)
		{
			if(tmp_start[parseInt(j/4)].charAt(j%4) < tmp_end[parseInt(j/4)].charAt(j%4))
			{	
				return -1;
			}
			else if (tmp_start[parseInt(j/4)].charAt(j%4) > tmp_end[parseInt(j/4)].charAt(j%4))
			{
				return 1;
			}
		}
		return 0;
	}

if (prefix.value!=128 || type=="same")
{
	// LanIP
	for (var i = 0; i < 8; i++)
	{
		// special case x:x:x:x:x:x:x:: would be considered as x:x:x:x:x:x:x:0000
		// special case ::x:x:x:x would be considered as 0000::x:x:x:x
		if (i != 0 && i != 7 && tmp_lan[i].length==0) // find ::
		{
			tmp = 8 - (tmp_lan.length - 1);
			for (var j = tmp_lan.length - 1 - i - 1; j >= 0; j--)
			{
				tmp_lan[i+tmp+j] = tmp_lan[(i+1)+j];
			}
			for (var j = 0; j < tmp; j++)
			{
				tmp_lan[i+j] = "0000";
			}
		}
		switch (tmp_lan[i].length)
		{
			case 0:
				tmp_lan[i]="0000";
				break;
			case 1:
				tmp_lan[i]="000"+tmp_lan[i];
				break;
			case 2:
				tmp_lan[i]="00"+tmp_lan[i];
				break;
			case 3:
				tmp_lan[i]="0"+tmp_lan[i];
				break;
			default:
				break;
		}
	}
	//alert(tmp_lan[0]+" "+tmp_lan[1]+" "+tmp_lan[2]+" "+tmp_lan[3]+" "+tmp_lan[4]+" "+tmp_lan[5]+" "+tmp_lan[6]+" "+tmp_lan[7]);

	if(type=="check_v6Lan_dhcp" || type=="check_range_DMZ")
	{
		//check start under lan
		for (var j=0; j<parseInt(prefix.value/4) ; j++ )
		{	
			if(tmp_start[parseInt(j/4)].charAt(j%4)!=tmp_lan[parseInt(j/4)].charAt(j%4))
			{		

				if (type=="check_v6Lan_dhcp")
				{
					top.AlertMsg("Alert", "Start IP address should not precede the IPv6 LAN IP Address.");
					return false;
				}
				else if(type=="check_range_DMZ")
				{
					top.AlertMsg("Alert", "Start IP address should not precede the IPv6 WAN IP Address.");
					return false;
				}
				return false;
			}
		}

		if (!CHeckIPv6_smallLarge(tmp_lan[parseInt(j/4)].charAt(j%4),tmp_start[parseInt(j/4)].charAt(j%4),prefix.value%4))
		{
				if (type=="check_v6Lan_dhcp")
				{
					top.AlertMsg("Alert", "Start IP address should not precede the IPv6 LAN IP Address.");
					return false;
				}
				else if(type=="check_range_DMZ")
				{
					top.AlertMsg("Alert", "Start IP address should not precede the IPv6 WAN IP Address.");
					return false;
				}
		}
		
		//check end under lan
		for (var j=0; j<parseInt(prefix.value/4) ; j++ )
		{	
			if(tmp_end[parseInt(j/4)].charAt(j%4)!=tmp_lan[parseInt(j/4)].charAt(j%4))
			{		

				if (type=="check_v6Lan_dhcp")
				{
					top.AlertMsg("Alert", "End IP address should not precede the IPv6 LAN IP Address.");
					return false;
				}
				else if(type=="check_range_DMZ")
				{
					top.AlertMsg("Alert", "End IP address should not precede the IPv6 WAN IP Address.");
					return false;
				}
				return false;
			}
		}

		if (!CHeckIPv6_smallLarge(tmp_lan[parseInt(j/4)].charAt(j%4),tmp_end[parseInt(j/4)].charAt(j%4),prefix.value%4))
		{
			if (type=="check_v6Lan_dhcp")
			{
				top.AlertMsg("Alert", "End IP address should not precede the IPv6 LAN IP Address.");
				return false;
			}
			else if(type=="check_range_DMZ")
			{
				top.AlertMsg("Alert", "End IP address should not precede the IPv6 WAN IP Address.");
				return false;
			}
		}
	}
	else if (type=="test_under")
	{
		for (var j=0; j<parseInt(prefix.value/4) ; j++ )
		{	
			if(tmp_start[parseInt(j/4)].charAt(j%4)!=tmp_lan[parseInt(j/4)].charAt(j%4))
			{		
				return false;
			}
		}

		if (!CHeckIPv6_smallLarge(tmp_lan[parseInt(j/4)].charAt(j%4),tmp_start[parseInt(j/4)].charAt(j%4),prefix.value%4))
		{
			return false;
		}
		return true;
	}
	else if (type=="check_v6Lan" || type=="check_v6DMZ" )
	{
		for (var j=0; j<parseInt(prefix.value/4) ; j++ )
		{	
			if(tmp_start[parseInt(j/4)].charAt(j%4)!=tmp_lan[parseInt(j/4)].charAt(j%4))
			{		
				samefrefix=false;
				break;
			}
		}

		if (samefrefix && !CHeckIPv6_smallLarge(tmp_lan[parseInt(j/4)].charAt(j%4),tmp_start[parseInt(j/4)].charAt(j%4),prefix.value%4))
		{
			samefrefix=false;
		}

	}
	else if (type=="same")
	{
		for (var j=0; j<32 ; j++ )
		{	
			if(tmp_start[parseInt(j/4)].charAt(j%4)!=tmp_lan[parseInt(j/4)].charAt(j%4))
			{		
				return false;
			}
		}

		return true;
	}
	

	//max dhcp available ip is 512 (DHCPV6_MAXAVAILABLEIP)
	var start_value=0,end_value=0,lan_value_s=0,lan_value_e=0,lan_value=0,ip_range_number=0;

	for (var j=parseInt(prefix.value/4); j<32 ;j++ )
	{	
		start_value=hex_to_ten(tmp_start[parseInt(j/4)].charAt(j%4));

		end_value=hex_to_ten(tmp_end[parseInt(j/4)].charAt(j%4));

		ip_range_number=ip_range_number+(end_value-start_value)*Math.pow(16,31-j);

		lan_value=hex_to_ten(tmp_lan[parseInt(j/4)].charAt(j%4));

		lan_value_s=lan_value_s+(lan_value-start_value)*Math.pow(16,31-j);
		lan_value_e=lan_value_e+(lan_value-end_value)*Math.pow(16,31-j);

	}

	if (type!="check_v6Lan" && type!="check_v6DMZ")
	{
		if (ip_range_number > DHCPV6_MAXAVAILABLEIP)
		{
			top.AlertMsg("Alert", "IP not more than "+DHCPV6_MAXAVAILABLEIP+"!");
			return false;
		}

		//check start ip is small than end ip
		if(ip_range_number<0)
		{	
			top.AlertMsg("Alert", "Start IP value should be smaller than End IP !");
			return false;
		}
	}



	//Lan and DHCP range has same prefix, lanip < dhcp_start or lanip > dhcp_end	
	if (samefrefix)
	{
		if (lan_value_s>=0 && lan_value_e<=0)
		{	
			if (type=="check_v6Lan_dhcp" || type=="check_v6Lan")
			{
				top.AlertMsg("Alert", aDhcpLanIpConflict);
				return false;
			}
			else if (type=="check_range_DMZ" ||  type=="check_v6DMZ" )
			{
				top.AlertMsg("Alert", aDMZSubnetConflict);
				return false;
			}
		}
	}

}
	return true;
}

function ip_range_start_get(ip,netmask,startip,output)
{
	var ipaddr=ip.value.split(".");
	var netmask=netmask.value.split(".");
	var start_ip=startip.value.split(".");
	
	if(parseInt(~netmask[3] + 1) == -1)
	{
		netmask[3] = 255;
	}
	else if(parseInt(~netmask[3] + 1) < -1)
	{
		netmask[3] = Math.abs(parseInt(~netmask[3] + 1));
	}
	
	var in_subnet = (ipaddr[3] & netmask[3]) == (start_ip[3] & netmask[3]) &&
			(1 || ipaddr[3] != start_ip[3]) &&
			(ipaddr[3] & netmask[3]) &&
			(ipaddr[3] & netmask[3]) != netmask[3];
	if ( !in_subnet )
	{
		start_ip[3] = (ipaddr[3] & netmask[3])+1;
	}
	if(output)
	{
		start_ip[3] = (ipaddr[3] & netmask[3])+1;
		return start_ip[3];
	}
	else
	{
		return in_subnet;
	}
}

function ip_range_end_get(ip,netmask,endip,output)
{
	var ipaddr=ip.value.split(".");
	var netmask=netmask.value.split(".");
	var end_ip=endip.value.split(".");
	var s_netmask=0;
	
	if(parseInt(netmask[3]) == 0)
	{
		netmask[3] = 255;
		s_netmask = 255;
	}
	else if(parseInt(~netmask[3] + 1) < -1)
	{
		s_netmask = 255-Math.abs(parseInt(~netmask[3] + 1));
		netmask[3] = Math.abs(parseInt(~netmask[3] + 1));
	}
	
	var in_subnet = (ipaddr[3] & netmask[3]) == (end_ip[3] & netmask[3]) &&
			(1 || ipaddr[3] != end_ip[3]) &&
			(ipaddr[3] & netmask[3]) &&
			(ipaddr[3] & netmask[3]) != netmask[3];
	if ( !in_subnet )
	{
		end_ip[3] = (ipaddr[3] & netmask[3] | s_netmask)-1;
	}
	if(output)
	{
		end_ip[3] = (ipaddr[3] & netmask[3] | s_netmask)-1;
		return end_ip[3];
	}
	else
	{
		return in_subnet;
	}
}

function PrintIPTypeTab(tab1,current1,tab2,current2,paddingtop){

	/*var isIE = navigator.userAgent.search("MSIE") > -1; 
    var isFirefox = navigator.userAgent.search("Firefox") > -1; 
    //var isOpera = navigator.userAgent.search("Opera") > -1; 
    //var isSafari = navigator.userAgent.search("Safari") > -1;

	if (isIE)
	{
		document.write('<div id="IEmenu"><ul>')
	}
	else if (isFirefox)
	{
		document.write('<div id="FFmenu"><ul>')
	}
	else
	{	
		document.write('<div id="menu"><ul>')
	}
	document.write('<ul>');

	//Tab1
	document.write('<li><a title="'+tab1+'" ');
	if (current1=='true')
		document.write(' class="current">');
	else
		document.write(' href="javascript:chChangeIPmode(1);">');
	document.write('<span>&nbsp;'+tab1+'&nbsp;</span></a></li>');
	
	//Tab2
	document.write('<li><a title="'+tab2+'" ');
	if (current2=='true')
		document.write(' class="current">');
	else
		document.write(' href="javascript:chChangeIPmode(2);">');
	document.write('<span>&nbsp;'+tab2+'&nbsp;</span></a></li>');

	document.write('</ul>');
	document.write('</div>');*/
	
	if(paddingtop)
		document.write('<tr><td style="padding-top:'+paddingtop+'px;">');
	else
		document.write('<tr><td>');
	document.write('<table width="100%" cellpadding="0" cellspacing="0" style="border-bottom:2px solid #7d7d7d;">');
	document.write('<tr>');
	document.write('<td width="8">');
	document.write('<img border="0" src="images/_blank.gif" width="1" height="15">');
	document.write('</td>');
	if (current1=='true')
		document.write('<td class="ipvxtabactive" onClick="">');
	else
		document.write('<td class="ipvxtabinactive" onClick="chChangeIPmode(1);">');
	
	document.write(tab1);
	document.write('</td>');
	
	if (current2=='true')
		document.write('<td class="ipvxtabactive" onClick="">');
	else
		document.write('<td class="ipvxtabinactive" onClick="chChangeIPmode(2);">');
	document.write(tab2);
	document.write('</td>');
	document.write('<td>');
	document.write('<img border="0" src="images/_blank.gif" width="1" height="15">');
	document.write('</td>');
	document.write('</tr>');	
	document.write('</table>');
	document.write('</td></tr>');

}

function Check_User_Input_Number(e)
{
	var keynum;
	
	if(window.event) // IE
		keynum = e.keyCode;
	else if(e.which) // Netscape/Firefox/Opera
		keynum = e.which;

	if (keynum < 48 || keynum > 57)
	{
		if (keynum != 8     //Backspace
		&&  keynum != 13)  //Enter  
		{
			return false;
		}	
	}	

	return true;
}
