function varargout = sensor(varargin)
% SENSOR M-file for sensor.fig
%      SENSOR, by itself, creates a new SENSOR or raises the existing
%      singleton*.
%
%      H = SENSOR returns the handle to a new SENSOR or the handle to
%      the existing singleton*.
%
%      SENSOR('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in SENSOR.M with the given input arguments.
%
%      SENSOR('Property','Value',...) creates a new SENSOR or raises the
%      existing singleton*.  Starting from the left, property value pairs 
are
%      applied to the GUI before sensor_OpeningFunction gets called.  An
%      unrecognized property name or invalid value makes property 
application
%      stop.  All inputs are passed to sensor_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help sensor

% Last Modified by GUIDE v2.5 12-Nov-2007 10:28:44

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @sensor_OpeningFcn, ...
                   'gui_OutputFcn',  @sensor_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


% --- Executes just before sensor is made visible.
function sensor_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to sensor (see VARARGIN)

% Choose default command line output for sensor
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes sensor wait for user response (see UIRESUME)
% uiwait(handles.figure1);



% --- Outputs from this function are returned to the command line.
function varargout = sensor_OutputFcn(hObject, eventdata, handles)
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;



function numNodes_Callback(hObject, eventdata, handles)
% hObject    handle to numNodes (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of numNodes as text
%        str2double(get(hObject,'String')) returns contents of numNodes as a 
double


% --- Executes during object creation, after setting all properties.
function numNodes_CreateFcn(hObject, eventdata, handles)
% hObject    handle to numNodes (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), 
get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on selection change in pattern.
function pattern_Callback(hObject, eventdata, handles)
% hObject    handle to pattern (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = get(hObject,'String') returns pattern contents as cell 
array
%        contents{get(hObject,'Value')} returns selected item from pattern


% --- Executes during object creation, after setting all properties.
function pattern_CreateFcn(hObject, eventdata, handles)
% hObject    handle to pattern (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: listbox controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), 
get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in ok.
function ok_Callback(hObject, eventdata, handles)
% hObject    handle to ok (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
numHandle = findobj('Tag','numNodes');
numNodes = str2double(get(numHandle,'String'));
sourceXHandle = findobj('Tag','sourceX');
sourceX = str2double(get(sourceXHandle, 'String'));
sourceYHandle = findobj('Tag','sourceY');
sourceY = str2double(get(sourceYHandle, 'String'));

if (mod(numNodes, floor(numNodes)) == 0 && numNodes ~= 0 && mod(sourceX, 
floor(sourceX)) == 0 && mod(sourceY, floor(sourceY)) == 0),
       patternHandle = findobj('Tag','pattern');
       pattern = get(patternHandle, 'Value');
       sinkHandle = findobj('Tag','sinkLoc');
       sinkLoc = get(sinkHandle,'Value');
       close(gcf)
       if (pattern == 1.0),
           num = floor(sqrt(numNodes));
           sensorXY = [];
           for x = 1:1:num,
               for y = 1:1:num,
                  plot(x, y, 'or', 'MarkerFaceColor','r', 'MarkerSize', 3)
                  sensorXY = [sensorXY; x y];
                  hold on
               end;
           end;
           ynum = num;
           xnum = num;
           if ((num * num) ~= numNodes),
               remNum = numNodes - (num * num);
               numRows = ceil(remNum / num);
               for x = 1:1:numRows,
                   xnum = xnum + 1;
                   if (remNum <= num),
                       n = remNum;
                   else
                       n = num;
                   end;
                   for y = 1:1:n,
                       plot(xnum, y, 'or', 'MarkerFaceColor','r', 
'MarkerSize', 3)
                       sensorXY = [sensorXY; xnum y];
                   end;
                   remNum = remNum - num;
               end;
           end;
           xnum = xnum + 1;
           ynum = ynum + 1;
           if (sinkLoc == 1.0),
              plot(0, floor(ynum / 2), 'ob', 'MarkerFaceColor', 'b', 
'MarkerSize', 3)
              sinkX = 0;
              sinkY = floor(ynum / 2);
           elseif (sinkLoc == 2.0),
              plot(xnum, floor(ynum / 2), 'ob', 'MarkerFaceColor', 'b', 
'MarkerSize', 3)
              sinkX = xnum;
              sinkY = floor(ynum / 2);
           elseif (sinkLoc == 3.0),
              plot(floor(xnum / 2), ynum, 'ob', 'MarkerFaceColor', 'b', 
'MarkerSize', 3)
              sinkX = floor(xnum / 2);
              sinkY = ynum;
           else
              plot(floor(xnum / 2), 0, 'ob', 'MarkerFaceColor', 'b', 
'MarkerSize', 3)
              sinkX = floor(xnum / 2);
              sinkY = 0;
           end;
           plot(sourceX, sourceY, 'og', 'MarkerFaceColor', 'g', 
'MarkerSize', 3)
           if (sourceX > xnum)
              xnum = sourceX;
           end;
           if (sourceY > ynum)
               ynum = sourceY;
           end;
           axis([0 xnum 0 ynum])
       else
           mod5 = mod(numNodes, 5);
           rem5 = 5 - mod5;
           num = numNodes + rem5;
           sensorXY = [];
           for n = 1:1:numNodes,
               x = floor(unifrnd(1,numNodes));
               y = floor(unifrnd(1,numNodes));
               plot(x, y, 'or', 'MarkerFaceColor','r', 'MarkerSize', 3)
               sensorXY = [sensorXY; x y];
               hold on
           end;
           if (sinkLoc == 1.0),
              plot(0, floor(num / 2), 'ob', 'MarkerFaceColor', 'b', 
'MarkerSize', 3)
              sinkX = 0;
              sinkY = floor(num / 2);
           elseif (sinkLoc == 2.0),
              plot(num, floor(num / 2), 'ob', 'MarkerFaceColor', 'b', 
'MarkerSize', 3)
              sinkX = num;
              sinkY = floor(num / 2);
           elseif (sinkLoc == 3.0),
              plot(floor(num / 2), num, 'ob', 'MarkerFaceColor', 'b', 
'MarkerSize', 3)
              sinkX = floor(num / 2);
              sinkY = num;
           else
              plot(floor(num / 2), 0, 'ob', 'MarkerFaceColor', 'b', 
'MarkerSize', 3)
              sinkX = floor(num / 2);
              sinkY = 0;
           end;
           plot(sourceX, sourceY, 'og', 'MarkerFaceColor', 'g', 
'MarkerSize', 3)
           if (sourceX > num)
              num = sourceX;
           end;
           if (sourceY > num)
               num = sourceY;
           end;
           axis([0 num 0 num])
       end;
       xlabel('X Position (m)')
       ylabel('Y Position (m)')
       grid on
       hold off
       save('data.mat', 'sensorXY', 'sinkX', 'sinkY', 'sourceX', 'sourceY')
else
    close(gcf)
    errordlg('You must enter a non-zero integer for number of nodes or 
source location.', 'Error')
end;

% --- Executes on button press in cancel.
function cancel_Callback(hObject, eventdata, handles)
% hObject    handle to cancel (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
close(gcf);



% --- Executes on selection change in sinkLoc.
function sinkLoc_Callback(hObject, eventdata, handles)
% hObject    handle to sinkLoc (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = get(hObject,'String') returns sinkLoc contents as cell 
array
%        contents{get(hObject,'Value')} returns selected item from sinkLoc


% --- Executes during object creation, after setting all properties.
function sinkLoc_CreateFcn(hObject, eventdata, handles)
% hObject    handle to sinkLoc (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: listbox controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), 
get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end





function SourceX_Callback(hObject, eventdata, handles)
% hObject    handle to SourceX (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of SourceX as text
%        str2double(get(hObject,'String')) returns contents of SourceX as a 
double


% --- Executes during object creation, after setting all properties.
function SourceX_CreateFcn(hObject, eventdata, handles)
% hObject    handle to SourceX (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), 
get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function sourceY_Callback(hObject, eventdata, handles)
% hObject    handle to sourceY (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of sourceY as text
%        str2double(get(hObject,'String')) returns contents of sourceY as a 
double


% --- Executes during object creation, after setting all properties.
function sourceY_CreateFcn(hObject, eventdata, handles)
% hObject    handle to sourceY (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), 
get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end





function sourceX_Callback(hObject, eventdata, handles)
% hObject    handle to sourceX (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of sourceX as text
%        str2double(get(hObject,'String')) returns contents of sourceX as a 
double


% --- Executes during object creation, after setting all properties.
function sourceX_CreateFcn(hObject, eventdata, handles)
% hObject    handle to sourceX (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), 
get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



