function varargout = trama(varargin)
%TRAMA M-file for trama.fig
%      TRAMA, by itself, creates a new TRAMA or raises the existing
%      singleton*.
%
%      H = TRAMA returns the handle to a new TRAMA or the handle to
%      the existing singleton*.
%
%      TRAMA('Property','Value',...) creates a new TRAMA using the
%      given property value pairs. Unrecognized properties are passed via
%      varargin to trama_OpeningFcn.  This calling syntax produces a
%      warning when there is an existing singleton*.
%
%      TRAMA('CALLBACK') and TRAMA('CALLBACK',hObject,...) call the
%      local function named CALLBACK in TRAMA.M with the given input
%      arguments.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help trama

% Last Modified by GUIDE v2.5 18-Nov-2007 16:35:27

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @trama_OpeningFcn, ...
                   'gui_OutputFcn',  @trama_OutputFcn, ...
                   'gui_LayoutFcn',  [], ...
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


% --- Executes just before trama is made visible.
function trama_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   unrecognized PropertyName/PropertyValue pairs from the
%            command line (see VARARGIN)

% Choose default command line output for trama
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes trama wait for user response (see UIRESUME)
% uiwait(handles.trama);
set(gca,'FontSize',9)
xlabel('X Position (m)')
ylabel('Y Position (m)')
grid on

% --- Outputs from this function are returned to the command line.
function varargout = trama_OutputFcn(hObject, eventdata, handles)
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes on selection change in scenario.
function scenario_Callback(hObject, eventdata, handles)
% hObject    handle to scenario (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = get(hObject,'String') returns scenario contents as cell 
array
%        contents{get(hObject,'Value')} returns selected item from scenario

%Enables Dynamics panel if scenario = 2.0;
scenario = get(hObject,'Value');
addHandle = findobj('Tag','add');
failHandle = findobj('Tag','fail');
clearHandle = findobj('Tag','clear');
atTimeHandle = findobj('Tag','atTime');

if (scenario == 2.0)
    set(addHandle, 'Enable', 'On');
    set(failHandle, 'Enable', 'On');
    set(clearHandle, 'Enable', 'On');
    set(atTimeHandle, 'Enable', 'On');
else
    set(addHandle, 'Enable', 'Off');
    set(failHandle, 'Enable', 'Off');
    set(clearHandle, 'Enable', 'Off');
    set(atTimeHandle, 'Enable', 'Off');
end;

% --- Executes during object creation, after setting all properties.
function scenario_CreateFcn(hObject, eventdata, handles)
% hObject    handle to scenario (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: listbox controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), 
get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on selection change in timeSync.
function timeSync_Callback(hObject, eventdata, handles)
% hObject    handle to timeSync (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = get(hObject,'String') returns timeSync contents as cell 
array
%        contents{get(hObject,'Value')} returns selected item from timeSync


% --- Executes during object creation, after setting all properties.
function timeSync_CreateFcn(hObject, eventdata, handles)
% hObject    handle to timeSync (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: listbox controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), 
get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



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


function transDuration_Callback(hObject, eventdata, handles)
% hObject    handle to transDuration (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of transDuration as text
%        str2double(get(hObject,'String')) returns contents of transDuration 
as a double


% --- Executes during object creation, after setting all properties.
function transDuration_CreateFcn(hObject, eventdata, handles)
% hObject    handle to transDuration (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), 
get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function signalDuration_Callback(hObject, eventdata, handles)
% hObject    handle to signalDuration (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of signalDuration as text
%        str2double(get(hObject,'String')) returns contents of 
signalDuration as a double


% --- Executes during object creation, after setting all properties.
function signalDuration_CreateFcn(hObject, eventdata, handles)
% hObject    handle to signalDuration (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), 
get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in init.
function init_Callback(hObject, eventdata, handles)
% hObject    handle to init (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

%Opens sensor.fig
trama = findobj('Tag', 'trama');
tramaPos = getpixelposition(trama);
h = openfig('sensor.fig','new');
setpixelposition(h, [tramaPos(1)+ 520 tramaPos(2)+ 230 435 331])

%Clears values for global variable addFail;
global addFail;
addFail = [];
myString = int2str(addFail);
selectionHandle = findobj('Tag','selection');
set(selectionHandle, 'String', myString);


% --- Executes on button press in startSim.
function startSim_Callback(hObject, eventdata, handles)
% hObject    handle to startSim (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

global stopSim;
global addFail;
transHandle = findobj('Tag','transDuration');
transDuration = str2double(get(transHandle, 'string'));
signalHandle = findobj('Tag','signalDuration');
signalDuration = str2double(get(signalHandle, 'string'));
radioHandle = findobj('Tag','radioRange');
radioRange = str2double(get(radioHandle, 'string'));
scenarioHandle = findobj('Tag','scenario');
scenario = get(scenarioHandle, 'value');
timeHandle = findobj('Tag','timeSync');
timeSync = get(timeHandle, 'value');

if (transDuration ~= 0 && signalDuration ~= 0 && radioRange ~= 0),
    if (timeSync == 1.0),

        % Processing for Time(T) = 0;
        timeCounter = 0;
        load data.mat
        if (scenario == 2.0),
           [na, ma] = size(addFail);
           hold on
           for x = 1:1:na,
              if (addFail(x, 3) <= timeCounter),
                  [n, m] = size(sensorXY);
                  found = 0;
                  for y = 1:1:n,
                     if (addFail(x, 1) == sensorXY(y, 1) && addFail(x, 2) == 
sensorXY(y, 2)),
                         found = 1;
                         foundRow = y;
                     end;
                  end;
                  if (addFail(x, 4) == 0),
                      if (found == 0),
                          plot(addFail(x, 1), addFail(x, 2), 'or', 
'MarkerFaceColor', 'r', 'MarkerSize', 3);
                          sensorXY = [sensorXY; [addFail(x, 1) addFail(x, 
2)]];
                      end;
                  elseif (found == 1)
                         h = findobj(gca, 'LineStyle', 'none', '-and', 
'Marker', 'o', '-and', 'XData', addFail(x, 1), '-and', 'YData', addFail(x, 
2));
                         delete(h);
                         sensorXY(foundRow,:)= [];
                  end;
              end;
           end;
           hold off
        end;
        tic
        all_sensorXY = [sensorXY; [sourceX sourceY]; [sinkX sinkY]];
        [n, m] = size(all_sensorXY);
        neighbors = [];
        for x = 1:1:n,
            currX = all_sensorXY(x, 1);
            currY = all_sensorXY(x, 2);
            for y = 1:1:n,
                d1 = currX - all_sensorXY(y, 1);
                d2 = currY - all_sensorXY(y, 2);
                if (d1 ~= 0 || d2 ~= 0),
                   if (d1 <= 2*radioRange && d1 >= -2*radioRange) && (d2 <= 
2*radioRange && d2 >= -2*radioRange),
                      neighbors = [neighbors; currX currY all_sensorXY(y, 1) 
all_sensorXY(y, 2)];
                   end;
                end;
            end;
        end;

        % Processing for Time(T) > 0
        currentX = sourceX;
        currentY = sourceY;
        stopSim = 0;

        while ((currentX ~= sinkX || currentY ~= sinkY) && stopSim ~= 1),
           [n, m] = size(neighbors);

            %random slot
           for x = 1:1:n,
               line([neighbors(x, 1) neighbors(x, 3)],[neighbors(x, 2) 
neighbors(x, 4)], 'LineStyle', '-.')
           end;
           h = findobj(gca,'Type','line', '-and', 'LineStyle', '-.');
           t = timer('StartDelay', signalDuration);
           t.TimerFcn = {'delete_fcn', h};
           start(t)
           wait(t)
           timeCounter = timeCounter + signalDuration;

           %scheduled slot
           least = 100;
           for x = 1:1:n,
               if (neighbors(x, 1) == currentX && neighbors(x, 2) == 
currentY),
                   d1 = abs(sinkX - neighbors(x, 3));
                   d2 = abs(sinkY - neighbors(x, 4));
                   d3 = abs(currentX - neighbors(x, 3));
                   d4 = abs(currentY - neighbors(x, 4));
                   distance = d1 + d2 + d3 + d4;
                   if (distance < least) && (d3 <= radioRange && d4 <= 
radioRange),
                      least = distance;
                      leastX = neighbors(x, 3);
                      leastY = neighbors(x, 4);
                      leastD3 = d3;
                      leastD4 = d4;
                   elseif (distance == least) && (d3 <= radioRange && d4 <= 
radioRange),
                      if (d3 + d4) < (leastD3 + leastD4),
                         least = distance;
                         leastX = neighbors(x, 3);
                         leastY = neighbors(x, 4);
                         leastD3 = d3;
                         leastD4 = d4;
                      elseif (neighbors(x,3) == sinkX && neighbors(x, 4) == 
sinkY),
                         leastX = neighbors(x, 3);
                         leastY = neighbors(x, 4);
                         break;
                      end;
                   end;
               end;
           end;
           if (least ~= 100),
              line([currentX leastX],[currentY leastY],'LineStyle','-', 
'LineWidth', 2.5)
              prevX = currentX;
              prevY = currentY;
              currentX = leastX;
              currentY = leastY;
           end;
           h = findobj(gca,'Type','line', '-and', 'LineStyle', '-.');
           t = timer('StartDelay', transDuration);
           t.TimerFcn = {'delete_fcn', h};
           start(t)
           wait(t)
           timeCounter = timeCounter + transDuration;

           if (scenario == 2.0),
              [na, ma] = size(addFail);
              hold on
              for x = 1:1:na,
                 if (addFail(x, 3) <= timeCounter),
                    [n, m] = size(sensorXY);
                    found = 0;
                    for y = 1:1:n,
                        if (addFail(x, 1) == sensorXY(y, 1) && addFail(x, 2) 
== sensorXY(y, 2)),
                            found = 1;
                            foundRow = y;
                        end;
                    end;
                    if (addFail(x, 4) == 0),
                        if (found == 0),
                            plot(addFail(x, 1), addFail(x, 2), 'or', 
'MarkerFaceColor', 'r', 'MarkerSize', 3);
                            sensorXY = [sensorXY; [addFail(x, 1) addFail(x, 
2)]];
                        end;
                    elseif (found == 1)
                         if (addFail(x, 1) == currentX && addFail(x, 2) == 
currentY),
                             h = findobj(gca,'Type','line', '-and', 
'LineStyle', '-', '-and', 'XData', [prevX currentX], '-and', 'YData', [prevY 
currentY]);
                             delete(h);
                             h = findobj(gca, 'LineStyle', 'none', '-and', 
'Marker', 'o', '-and', 'XData', addFail(x, 1), '-and', 'YData', addFail(x, 
2));
                             delete(h);
                             currentX = prevX;
                             currentY = prevY;
                             sensorXY(foundRow,:)= [];
                         else
                             h = findobj(gca, 'LineStyle', 'none', '-and', 
'Marker', 'o', '-and', 'XData', addFail(x, 1), '-and', 'YData', addFail(x, 
2));
                             delete(h);
                             sensorXY(foundRow,:)= [];
                         end;
                    end;
                 end;
              end;
              hold off

              all_sensorXY = [sensorXY; [sourceX sourceY]; [sinkX sinkY]];
              [n, m] = size(all_sensorXY);
              neighbors = [];
              for x = 1:1:n,
                 currX = all_sensorXY(x, 1);
                 currY = all_sensorXY(x, 2);
                 for y = 1:1:n,
                    d1 = currX - all_sensorXY(y, 1);
                    d2 = currY - all_sensorXY(y, 2);
                    if (d1 ~= 0 || d2 ~= 0),
                       if (d1 <= 2*radioRange && d1 >= -2*radioRange) && (d2 
<= 2*radioRange && d2 >= -2*radioRange),
                          neighbors = [neighbors; currX currY 
all_sensorXY(y, 1) all_sensorXY(y, 2)];
                       end;
                    end;
                 end;
              end;
           end;
        end;
    end;
    t = toc;
    latencyHandle = findobj('Tag','latency');
    set(latencyHandle, 'string', num2str(t))
else
    errordlg('You must enter non-zero values for the duration times and 
radio range.', 'Error')
end;


% --- Executes on button press in stopSim.
function stopSim_Callback(hObject, eventdata, handles)
% hObject    handle to stopSim (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global stopSim;
stopSim = 1;
global addFail;
addFail = [];
myString = int2str(addFail);
selectionHandle = findobj('Tag','selection');
set(selectionHandle, 'String', myString);


function transRate_Callback(hObject, eventdata, handles)
% hObject    handle to transRate (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of transRate as text
%        str2double(get(hObject,'String')) returns contents of transRate as 
a double


% --- Executes during object creation, after setting all properties.
function transRate_CreateFcn(hObject, eventdata, handles)
% hObject    handle to transRate (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), 
get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function radioRange_Callback(hObject, eventdata, handles)
% hObject    handle to radioRange (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of radioRange as text
%        str2double(get(hObject,'String')) returns contents of radioRange as 
a double


% --- Executes during object creation, after setting all properties.
function radioRange_CreateFcn(hObject, eventdata, handles)
% hObject    handle to radioRange (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), 
get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in add.
function add_Callback(hObject, eventdata, handles)
% hObject    handle to add (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global addFail;
atTimeHandle = findobj('Tag','atTime');
again = 1;
xy = [];
n = 0;
while again == 1
    [xi, yi, again] = ginput(1);
    n = n+1;
    xi = ceil(abs(xi));
    yi = ceil(abs(yi));
    atTime = str2num(get(atTimeHandle, 'string'));
    xy(n,:) = [xi;yi;atTime;0];
end;
addFail = [addFail;xy];
myString = int2str(addFail);
selectionHandle = findobj('Tag','selection');
set(selectionHandle, 'String', myString);


% --- Executes on button press in fail.
function fail_Callback(hObject, eventdata, handles)
% hObject    handle to fail (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global addFail;
atTimeHandle = findobj('Tag','atTime');
again = 1;
xy = [];
n = 0;
while again == 1
    [xi, yi, again] = ginput(1);
    n = n+1;
    xi = ceil(abs(xi));
    yi = ceil(abs(yi));
    atTime = str2num(get(atTimeHandle, 'string'));
    xy(n,:) = [xi;yi;atTime;1];
end;
addFail = [addFail;xy];
myString = int2str(addFail);
selectionHandle = findobj('Tag','selection');
set(selectionHandle, 'String', myString);

% --- Executes on selection change in selection.
function selection_Callback(hObject, eventdata, handles)
% hObject    handle to selection (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = get(hObject,'String') returns selection contents as cell 
array
%        contents{get(hObject,'Value')} returns selected item from selection


% --- Executes during object creation, after setting all properties.
function selection_CreateFcn(hObject, eventdata, handles)
% hObject    handle to selection (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: listbox controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), 
get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function atTime_Callback(hObject, eventdata, handles)
% hObject    handle to atTime (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of atTime as text
%        str2double(get(hObject,'String')) returns contents of atTime as a 
double


% --- Executes during object creation, after setting all properties.
function atTime_CreateFcn(hObject, eventdata, handles)
% hObject    handle to atTime (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), 
get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in clear.
function clear_Callback(hObject, eventdata, handles)
% hObject    handle to clear (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of clear
global addFail;
addFail = [];
myString = int2str(addFail);
selectionHandle = findobj('Tag','selection');
set(selectionHandle, 'String', myString);



