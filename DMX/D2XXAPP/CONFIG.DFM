�
 TCONFIGFORM 0q  TPF0TConfigForm
ConfigFormLeft� TopkWidth�HeightVCaptionConfigurationColor	clBtnFaceFont.CharsetDEFAULT_CHARSET
Font.ColorclWindowTextFont.Height�	Font.NameMS Sans Serif
Font.Style OldCreateOrderPixelsPerInch`
TextHeight 	TGroupBox	OptionsGBLeft TopWidth� Height� CaptionDataFont.CharsetDEFAULT_CHARSET
Font.ColorclWindowTextFont.Height�	Font.NameMS Sans Serif
Font.Style 
ParentFontTabOrder  TLabelLabel1LeftTop Width3HeightCaption	BaudRate:Font.CharsetDEFAULT_CHARSET
Font.ColorclWindowTextFont.Height�	Font.NameMS Sans Serif
Font.Style 
ParentFont  TLabelLabel2LeftTop8Width.HeightCaption
Data Bits:Font.CharsetDEFAULT_CHARSET
Font.ColorclWindowTextFont.Height�	Font.NameMS Sans Serif
Font.Style 
ParentFont  TLabelLabel3LeftTopPWidth-HeightCaption
Stop Bits:Font.CharsetDEFAULT_CHARSET
Font.ColorclWindowTextFont.Height�	Font.NameMS Sans Serif
Font.Style 
ParentFont  TLabelLabel4Left(TophWidthHeightCaptionParity:Font.CharsetDEFAULT_CHARSET
Font.ColorclWindowTextFont.Height�	Font.NameMS Sans Serif
Font.Style 
ParentFont  	TComboBox
BaudRateCBLeftHTop WidthIHeight
ItemHeightItems.Strings300600120024004800960014400192003840057600115200230400460800921600 TabOrder Text115200  	TComboBox
DataBitsCBLeftHTop8WidthIHeight
ItemHeightItems.Strings7 bits8 bits TabOrderText8 bits  	TComboBox
StopBitsCBLeftHTopPWidthIHeight
ItemHeightItems.Strings1 bit2 bits TabOrderText1 bit  	TComboBoxParityCBLeftHTophWidthIHeight
ItemHeightItems.StringsNoneOddEvenMarkSpace TabOrderTextNone   	TGroupBoxFlowControlGBLeft� TopWidth� Height� CaptionHandshakingTabOrder TLabelLabel5LeftTop Width=HeightCaptionFlow Control:  	TComboBoxFlowControlCBLeftPTop WidthIHeight
ItemHeightItems.StringsNoneRTS/CTSDTR/DSRXON/XOFF TabOrder TextNone  	TCheckBoxRtsCBLeft0Top`Width)Height	AlignmenttaLeftJustifyCaptionRTS: Checked	State	cbCheckedTabOrder  	TCheckBoxDtrCBLeft0ToppWidth)Height	AlignmenttaLeftJustifyCaptionDTR:Checked	State	cbCheckedTabOrder   	TGroupBoxSpecialCharsCBLeftXTopWidth� Height� CaptionSpecial CharsTabOrder TLabelLabel6LeftTop WidthHeightCaptionXON:  TLabelLabel7LeftTop8WidthHeightCaptionXOFF:  TLabelEventCharLabelLeftTopPWidthHeightCaptionEvent:Enabled  TLabelErrorCharLabelLeftTophWidthHeightCaptionError:Enabled  	TMaskEditXonMELeft8Top Width!HeightEditMaskAA	MaxLengthTabOrder Text11  	TMaskEditXoffMELeft8Top8Width!HeightEditMaskAA	MaxLengthTabOrderText13  	TMaskEditEventCharMELeft8TopPWidth!HeightEnabledEditMaskAA	MaxLengthTabOrderText00  	TMaskEditErrorCharMELeft8TophWidth!HeightEnabledEditMaskAA	MaxLengthTabOrderText00  	TCheckBoxEventCharCBLeft`TopPWidthHeightTabOrderOnClickEventCharCBClick  	TCheckBoxErrorCharCBLeft`TophWidthHeightCaptionErrorCharCBTabOrderOnClickErrorCharCBClick   TButtonButton1Left� TopWidthKHeightCaptionOKModalResultTabOrder  TButtonButton2LeftTopWidthKHeightCaptionCancelModalResultTabOrder  	TGroupBox
TimeoutsGBLeft Top� Width� HeightWCaptionTimeoutsTabOrder TLabelLabel8Left TopWidth(HeightCaptionRX (ms):  TLabelLabel9Left Top0Width'HeightCaptionTX (ms):  	TMaskEditRxTimeoutMELeftHTopWidthIHeightEditMask099999	MaxLengthTabOrder Text300     	TMaskEditTxTimeoutMELeftHTop0WidthIHeightEditMask099999	MaxLengthTabOrderText300      	TGroupBoxEventsGBLeft� Top� Width� HeightWCaptionEventsTabOrder 	TCheckBoxRxCharCBLeftTopWidth� Height	AlignmenttaLeftJustifyCaptionFT_EVENT_RXCHARTabOrder   	TCheckBoxModemStatCBLeftTop(Width� Height	AlignmenttaLeftJustifyCaptionFT_EVENT_MODEM_STATTabOrder    