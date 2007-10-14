unit uMain;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, Buttons, Grids, ExtCtrls, ShellApi;

const Selected = 'X';
const NotSelected = '-';

type
  TMain = class(TForm)
    Grid: TStringGrid;
    GroupBox1: TGroupBox;
    Panel1: TPanel;
    bTransform: TBitBtn;
    eDirXML: TEdit;
    Label1: TLabel;
    Label2: TLabel;
    eDirXSL: TEdit;
    Label3: TLabel;
    eDirDestination: TEdit;
    Label4: TLabel;
    eTransformerPath: TEdit;
    bRefresh: TBitBtn;
    Label5: TLabel;
    eTransformCommand: TEdit;
    Label6: TLabel;
    eXSLFileExclusion: TEdit;
    procedure FormShow(Sender: TObject);
    procedure bRefreshClick(Sender: TObject);
    procedure GridMouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure bTransformClick(Sender: TObject);
  private
    { Private declarations }
    procedure FindDevices;
    procedure FindFeatures;
    procedure FillSelections;
    procedure InvertCellSelection(Col: integer; Row: integer);
    procedure Transform;
    function CreateCommand(Device: string; Feature: string): string;
  public
    { Public declarations }
  end;

var
  Main: TMain;

implementation

{$R *.dfm}


{-------------------------------------------------------------------------------
   General functions
-------------------------------------------------------------------------------}

function AddDirEnd(s: string): string;
begin
  if Copy(s, Length(s), 1) <> '\' then s := s + '\';
  AddDirEnd := s;
end;

function ExtractFileNameOnly(filename: string): string;
var s: string;
begin
  s := ExtractFileName(filename);
  Result := Copy(s, 1, Length(s)-Length(ExtractFileExt(s)));
end;


{-------------------------------------------------------------------------------
   Initialization
-------------------------------------------------------------------------------}

procedure TMain.FormShow(Sender: TObject);
var
   Path: string;
begin
  Path := AddDirEnd(ExtractFilePath(Application.ExeName))+'../';

  eDirXML.Text := 'DeviceDesciptions';
  eDirXSL.Text := 'XSL';
  eXSLFileExclusion.Text := 'General';
  eTransformerPath.Text := 'Saxon\Transform.exe';
  eDirDestination.Text := '../AVRCppLib';
  eTransformCommand.Text := '%Transformer -t %XML %XSL > %Code';

  Grid.ColWidths[0] := 120;

  FindDevices;
  FindFeatures;
  FillSelections;
end;


{-------------------------------------------------------------------------------
   Devices search
-------------------------------------------------------------------------------}

procedure TMain.FindDevices;
var
    SRec: TSearchRec;
    Found: integer;
    Row: integer;
begin
  Grid.RowCount := 1;
  Row := 1;

  Found := FindFirst(AddDirEnd(eDirXML.Text) + '*.xml', 0, SRec);
  while Found = 0 do
  begin
    Grid.RowCount := Grid.RowCount + 1;
    Grid.Cells[0, Row] := ExtractFileNameOnly(Srec.Name);

    Found := FindNext(SRec);
    Inc(Row);
  end;
  FindClose(SRec);

  if Grid.RowCount > 1 then Grid.FixedRows := 1;
end;

procedure TMain.FindFeatures;
var
    SRec: TSearchRec;
    Found: integer;
    Col: integer;
    Name: string;
begin
  Grid.ColCount := 1;
  Col := 1;

  Found := FindFirst(AddDirEnd(eDirXSL.Text) + '*.xsl', 0, SRec);
  while Found = 0 do
  begin
    Name := ExtractFileNameOnly(Srec.Name);

    if Pos(Name, eXSLFileExclusion.Text) = 0 then
    begin
      Grid.ColCount := Grid.ColCount + 1;
      Grid.Cells[Col, 0] := Name;
      Inc(Col);
    end;

    Found := FindNext(SRec);
  end;
  FindClose(SRec);

  if Grid.ColCount > 1 then Grid.FixedCols := 1;
end;

procedure TMain.FillSelections;
var
   X, Y: integer;
begin
  for X := 1 to Grid.ColCount - 1 do
  begin
    for Y := 1 to Grid.RowCount - 1 do
    begin
      Grid.Cells[X, Y] := NotSelected;
    end;
  end;
end;


{-------------------------------------------------------------------------------
   Grid selection change
-------------------------------------------------------------------------------}

procedure TMain.GridMouseDown(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
var
   Col, Row: integer;
   Nr: integer;
begin
  Grid.MouseToCell(X, Y, Col, Row);

  if (Col > 0) and (Row > 0) then
    InvertCellSelection(Col, Row);

  if (Col = 0) and (Row > 0) then
    for Nr := 1 to Grid.ColCount - 1 do
      InvertCellSelection(Nr, Row);

  if (Col > 0) and (Row = 0) then
    for Nr := 1 to Grid.RowCount - 1 do
      InvertCellSelection(Col, Nr);
end;

procedure TMain.InvertCellSelection(Col: integer; Row: integer);
begin
  if Grid.Cells[Col, Row] = Selected then
    Grid.Cells[Col, Row] := NotSelected
  else
    Grid.Cells[Col, Row] := Selected;
end; 


{-------------------------------------------------------------------------------
   Transforming
-------------------------------------------------------------------------------}

procedure TMain.Transform;
var
   Row, Col: integer;
   LocalPath: string;
   Path: string;
   Feature: string;
   Cmd, Command: string;
   CmdChar: PChar;
   CmdLen: cardinal;
begin
  CmdLen := 255;
  GetMem(CmdChar, CmdLen);
  GetEnvironmentVariable(PChar('COMSPEC'), CmdChar, CmdLen);
  Cmd := StrPas(CmdChar);
  
  LocalPath := ExtractFilePath(Application.ExeName);

  for Row := 1 to Grid.RowCount - 1 do
  begin
    Path := AddDirEnd(eDirDestination.Text) + Grid.Cells[0, Row];

    // Create directory if it doesn't exist
    if not DirectoryExists(Path) then MkDir(Path);

    // Features to create
    for Col := 1 to Grid.ColCount - 1 do
    begin
      if Grid.Cells[Col, Row] = Selected then
      begin
        Command := CreateCommand(Grid.Cells[0, Row], Grid.Cells[Col, 0]);
        ShellExecute(Application.Handle, 'open', PChar(Cmd), PChar('/C ' + Command), PChar(LocalPath), SW_SHOW);
      end;
    end;
  end;
end;

function TMain.CreateCommand(Device: string; Feature: string): string;
var
   Command: string;
begin
  Command := eTransformCommand.Text;
  
  Command := StringReplace(Command, '%Transformer', eTransformerPath.Text, [rfReplaceAll]);
  Command := StringReplace(Command, '%XML', AddDirEnd(eDirXML.Text) + Device + '.xml', [rfReplaceAll]);
  Command := StringReplace(Command, '%XSL', AddDirEnd(eDirXSL.Text) + Feature + '.xsl', [rfReplaceAll]);
  Command := StringReplace(Command, '%Code', AddDirEnd(eDirDestination.Text) + AddDirEnd(Device) + Feature + '.h', [rfReplaceAll]);

  Result := Command;
end;


{-------------------------------------------------------------------------------
   Buttons
-------------------------------------------------------------------------------}

procedure TMain.bRefreshClick(Sender: TObject);
begin
  FindDevices();
  FindFeatures;
  FillSelections;
end;

procedure TMain.bTransformClick(Sender: TObject);
begin
  Transform;
end;

end.
