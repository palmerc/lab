function varargout = tramaInterface(varargin)
clc;
% TRAMAINTERFACE M-file for tramaInterface.fig
%      TRAMAINTERFACE, by itself, creates a new TRAMAINTERFACE or raises the existing
%      singleton*.
%
%      H = TRAMAINTERFACE returns the handle to a new TRAMAINTERFACE or the handle to
%      the existing singleton*.
%
%      TRAMAINTERFACE('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in TRAMAINTERFACE.M with the given input arguments.
%
%      TRAMAINTERFACE('Property','Value',...) creates a new TRAMAINTERFACE or raises
%      the existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before tramaInterface_OpeningFunction gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to tramaInterface_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help tramaInterface

% Last Modified by GUIDE v2.5 28-Oct-2007 13:31:38

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @tramaInterface_OpeningFcn, ...
                   'gui_OutputFcn',  @tramaInterface_OutputFcn, ...
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

% --- Executes just before tramaInterface is made visible.
function tramaInterface_OpeningFcn(hObject, eventdata, handles, varargin)

%clear workspace;
%clc;
handles.output = hObject;
guidata(hObject, handles);

axes(handles.axes3);


Logo=imshow('Logo1.jpg');
axis([get(Logo,'XData') get(Logo,'YData')]);


initialize_gui(hObject, handles, false);



% UIWAIT makes tramaInterface wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = tramaInterface_OutputFcn(hObject, eventdata, handles)
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;

% --- Executes on button press in plot.
function floodnodes_Callback(hObject, eventdata, handles)
% hObject    handle to plot (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
Level=1;
nolevel=0;
set(handles.statusbar, 'String', 'Flooding');
message = 0;
nodelevel=[];
plotdata = handles.plotdata;
A=0:handles.x_length;
B=(0:handles.y_length)';

axes(handles.axes1);
cla;

axis([0 ceil(A(1,handles.x_length))+1 0 ceil(B(handles.y_length,1))+1]);
%Node  Structure   
for i=1:handles.count
    x=plotdata(i).X;
    y=plotdata(i).Y;
    hold on
    plot(x,y,'s','MarkerSize',5,'MarkerEdgeColor','k','MarkerFaceColor','g','LineWidth',2)
    plotdata(i).parent = -1;
    plotdata(i).child = [];
end

sourcenodes = handles.beginnode;
tovisitnodes = 1:handles.count;
tovisitnodes(:,sourcenodes) = [];
parentnode(1,100)=0;%modify 100
children=zeros(1,100);
newsourcenodes = [];
visitagainnodes = [];

colors = ['c' 'm' 'b' 'r' 'y' 'g' 'k'];
usecolor = 1;

nodelevel(1,sourcenodes)=Level;
power_transmitted = 10.^(handles.trans_power/10);
power_threshold = 10.^(handles.flood_threshold/10);
%Flooding
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
while (isempty(sourcenodes) < 1)
    arraysize = size(sourcenodes);
    if(nolevel==0)
        nolevel=arraysize(1,2);
    end
    randomnum = randint(1,1,[1,arraysize(1,2)]);
    currnode = sourcenodes(1,randomnum);
    parentnode(1,currnode)=currnode;
    numberofnodestovisit = size(tovisitnodes);
    for n=1:1:(numberofnodestovisit(1,2))
        node = tovisitnodes(1,n);  
       
        dist = sqrt((plotdata(currnode).X-plotdata(node).X)^2 +...
               (plotdata(currnode).Y-plotdata(node).Y)^2);

        received_power = power_transmitted / (dist^handles.factor);
        if (received_power >= power_threshold)
            if (likelyprobability(dist, handles) == 1)
                newsourcenodes = [newsourcenodes node];
                xs = [plotdata(currnode).X plotdata(node).X];
                ys = [plotdata(currnode).Y plotdata(node).Y];
                plot(xs, ys, colors(1,usecolor), 'LineWidth',2);
                plotdata(currnode).child = [plotdata(currnode).child node];
                plotdata(node).parent = currnode;
                children(1,node)=currnode;
            else
                visitagainnodes = [node visitagainnodes];
            end
        else
            visitagainnodes = [node visitagainnodes];
        end
    end
    
    nolevel=nolevel-1;
    if(nolevel==0)
        Level=Level+1;
        nodelevel(1,newsourcenodes)=Level;
        
    end
    message = message + 1;
    status = mod(message, 3);
    if (status == 0)
        set(handles.statusbar, 'String', 'Flooding.');
    elseif (status == 1)
        set(handles.statusbar, 'String', 'Flooding..');
    else
        set(handles.statusbar, 'String', 'Flooding...');
    end
    tovisitnodes = visitagainnodes;
    visitagainnodes = [];
    sourcenodes(:,randomnum) = [];
    if isempty(sourcenodes);
        usecolor = mod(usecolor+1, 7);
        if (usecolor == 0)
            usecolor = 7;
        end
        sourcenodes = newsourcenodes;
        newsourcenodes = [];
        pause(handles.waittime);
        drawnow;
    end
end
handles.Level=Level;
handles.nodecount1=handles.count;
handles.nodelevel=[];
handles.nodelevel(1:handles.nodecount1)=nodelevel(1:handles.nodecount1);
handles.parentnode=[];
handles.children=[];
handles.parentnode(1:handles.nodecount1)=parentnode(1:handles.nodecount1);
handles.children(1:handles.nodecount1)=children(1:handles.nodecount1);
handles.plotdata = plotdata;
handles.power_transmitted=power_transmitted;
set(handles.ExaminePointer, 'Enable', 'on');
set(handles.Path, 'Enable', 'on');
set(handles.statusbar, 'String', 'Flooding...Done.');

% Update handles structure
guidata(handles.figure1, handles);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% --- Executes on button press in TPSN_sync.
function TPSN_sync_Callback(hObject, eventdata, handles)
% hObject    handle to TPSN_sync (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
%Globalvars();
nodecount2=handles.nodecount1;
message=0;
Level=handles.Level
parentnode(1:nodecount2)=handles.parentnode(1:nodecount2);
nodelevel(1:nodecount2)=handles.nodelevel(1:nodecount2);9948566876
children(1:nodecount2)=handles.children;
plotdata=handles.plotdata;
%power_transmitted=handles.power_transmitted;
%nodecount2=handles.nodecount2;
%handles.plotdata=plotdata;
pauseInt=0.2;
for i=1:Level-1
     for j = 1 :nodecount2
         if (nodelevel(j) == i)
             index=find(children==j);
            TPSNrequest(j,index,plotdata);
         end
     end
     if (pauseInt ~= 0)
         pause(pauseInt);
     end        
     
     for j = 1 : nodecount2
        if (nodelevel(j) == i)
             index=find(children==j);
              Ack(j,index,plotdata);
        end
     end                
     if (pauseInt ~= 0)
        pause(pauseInt);
     end
     for j = 1 : nodecount2
         if (nodelevel(j) == i)
              index=find(children==j);
             TPSNfinish(j,index,plotdata);
         end
     end
     
       message = message + 1;
    status = mod(message, 3);
    if (status == 0)
        set(handles.statusbar, 'String', 'Synchronizing.');
    elseif (status == 1)
        set(handles.statusbar, 'String', 'Synchronizing..');
    else
        set(handles.statusbar, 'String', 'Synchronizing...');
    end
     
end



handles.plotdata = plotdata;
set(handles.ExaminePointer, 'Enable', 'on');
set(handles.Path, 'Enable', 'on');
set(handles.statusbar, 'String', 'TPSN...Done.');

% Update handles structure
guidata(handles.figure1, handles);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% --------------------------------------------------------------------
function initialize_gui(fig_handle, handles, isreset)
% If the metricdata field is present and the reset flag is false, it means
% we are we are just re-initializing a GUI by calling it from the cmd line
% while it is up. So, bail out as we dont want to reset the data.

handles.plotmode = 0;
handles.x_length = 1000;
handles.y_length = 1000;
handles.count = 100;
handles.beginnode = 1;
handles.flood_threshold = -90;
handles.trans_power = -5;
handles.factor = 3.5;
handles.waittime = 0.5;
handles.goodreceptiondistance = 75;
handles.badreceptiondistance = reachabledistance(handles.trans_power,...
                                                 handles.flood_threshold,...
                                                 handles.factor);
set(handles.weakrange,'String', handles.badreceptiondistance);


% Update handles structure
guidata(handles.figure1, handles);
% --------------------------------------------------------------------
function xnodes_Callback(hObject, eventdata, handles)
% hObject    handle to xnodes (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of xnodes as text
%        str2double(get(hObject,'String')) returns contents of xnodes as a double
x_length = str2double(get(hObject, 'String'));
if isnan(x_length)
    set(hObject, 'String', 1);
    errordlg('Input must be a number','Error');
end
if (x_length < 1) 
    set(hObject, 'String', 1);
    errordlg('Input must be greater than 1','Error');
end
% Save the new x_length value
handles.x_length = str2double(get(hObject, 'String'));
guidata(hObject,handles)


% --- Executes during object creation, after setting all properties.
function xnodes_CreateFcn(hObject, eventdata, handles)
% hObject    handle to xnodes (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

function ynodes_Callback(hObject, eventdata, handles)
% hObject    handle to ynodes (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of ynodes as text
%        str2double(get(hObject,'String')) returns contents of ynodes as a double

y_length = str2double(get(hObject, 'String'));
if isnan(y_length)
    set(hObject, 'String', 1);
    errordlg('Input must be a number','Error');
end
if (y_length < 1) 
    set(hObject, 'String', 1);
    errordlg('Input must be greater than 1','Error');
end
% Save the new y_length value
handles.y_length = str2double(get(hObject, 'String'));
guidata(hObject,handles)

% --- Executes during object creation, after setting all properties.
function ynodes_CreateFcn(hObject, eventdata, handles)
% hObject    handle to ynodes (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

function nodecount_Callback(hObject, eventdata, handles)
% hObject    handle to nodecount1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of nodecount1 as text
%        str2double(get(hObject,'String')) returns contents of nodecount1 as a double

count = str2double(get(hObject, 'String'));
if isnan(count)
    set(hObject, 'String', 1);
    errordlg('Input must be a number','Error');
end
if (count < 1)
    set(hObject, 'String', 1);
    errordlg('Input must be greater than 1','Error');
end
% Save the new count value
handles.count = str2double(get(hObject, 'String'));

if (handles.beginnode > handles.count)
    handles.beginnode = 1;
    set(handles.startnode,'String', handles.beginnode);
end
guidata(hObject,handles)


% --- Executes during object creation, after setting all properties.
function nodecount_CreateFcn(hObject, eventdata, handles)
% hObject    handle to nodecount1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


function startnode_Callback(hObject, eventdata, handles)
% hObject    handle to startnode (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of startnode as text
%        str2double(get(hObject,'String')) returns contents of startnode as a double

beginnode = str2double(get(hObject, 'String'));
if isnan(beginnode)
    set(hObject, 'String', 1);
    errordlg('Input must be a number','Error');
end
if ((beginnode < 1) || (beginnode > handles.count))
    set(hObject, 'String', 1);
    errordlg('Input must between 1 and the total number of nodes','Error');
end
% Save the new beginnode value
handles.beginnode = str2double(get(hObject, 'String'));
guidata(hObject,handles)


% --- Executes during object creation, after setting all properties.
function startnode_CreateFcn(hObject, eventdata, handles)
% hObject    handle to startnode (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

function threshold_Callback(hObject, eventdata, handles)
% hObject    handle to threshold (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of threshold as text
%        str2double(get(hObject,'String')) returns contents of threshold as a double

threshold = str2double(get(hObject, 'String'));
if isnan(threshold)
    set(hObject, 'String', -20);
    errordlg('Input must be a number','Error');
end
% Save the new threshold value
handles.flood_threshold = str2double(get(hObject, 'String'));
handles.badreceptiondistance = reachabledistance(handles.trans_power,...
                                                 handles.flood_threshold,...
                                                 handles.factor);
set(handles.weakrange,'String', handles.badreceptiondistance);
guidata(hObject,handles)


% --- Executes during object creation, after setting all properties.
function threshold_CreateFcn(hObject, eventdata, handles)
% hObject    handle to threshold (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

function power_Callback(hObject, eventdata, handles)
% hObject    handle to power (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of power as text
%        str2double(get(hObject,'String')) returns contents of power as a double

trans_power = str2double(get(hObject, 'String'));
if isnan(trans_power)
    set(hObject, 'String', 0);
    errordlg('Input must be a number','Error');
end
% Save the new power value
handles.trans_power = str2double(get(hObject, 'String'));;
handles.badreceptiondistance = reachabledistance(handles.trans_power,...
                                                 handles.flood_threshold,...
                                                 handles.factor);
set(handles.weakrange,'String', handles.badreceptiondistance);
guidata(hObject,handles)

% --- Executes during object creation, after setting all properties.
function power_CreateFcn(hObject, eventdata, handles)
% hObject    handle to power (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function environmentfactor_Callback(hObject, eventdata, handles)
% hObject    handle to environmentfactor (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of environmentfactor as text
%        str2double(get(hObject,'String')) returns contents of environmentfactor as a double

enivironment_factor = str2double(get(hObject, 'String'));
if isnan(enivironment_factor)
    set(hObject, 'String', 1);
    errordlg('Input must be a number','Error');
end
if (enivironment_factor < 1) 
    set(hObject, 'String', 1);
    errordlg('Input must be greater than 1','Error');
end
% Save the new y_length value
handles.factor = str2double(get(hObject, 'String'));
handles.badreceptiondistance = reachabledistance(handles.trans_power,...
                                                 handles.flood_threshold,...
                                                 handles.factor);
set(handles.weakrange,'String', handles.badreceptiondistance);

guidata(hObject,handles)

% --- Executes during object creation, after setting all properties.
function environmentfactor_CreateFcn(hObject, eventdata, handles)
% hObject    handle to environmentfactor (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in pushbutton9.
function reset_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton9 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

handles.plotmode = 0;
handles.x_length = 1000;
handles.y_length = 1000;
handles.count = 100;
handles.beginnode = 1;
handles.flood_threshold = -90;
handles.trans_power = -5;
handles.factor = 3.5;
handles.waittime = 0.5;
handles.goodreceptiondistance = 75;
handles.badreceptiondistance = reachabledistance(handles.trans_power,...
                                                 handles.flood_threshold,...
                                                 handles.factor);

set(handles.xnodes,'String', handles.x_length);
set(handles.ynodes,'String', handles.y_length);
set(handles.nodecount,'String', handles.count);
set(handles.startnode,'String', handles.beginnode);
set(handles.threshold,'String', handles.flood_threshold);
set(handles.power,'String', handles.trans_power);
set(handles.environmentfactor,'String', handles.factor);
set(handles.weakrange,'String', handles.badreceptiondistance);
set(handles.goodrange,'String', handles.goodreceptiondistance);

guidata(handles.figure1, handles);

% --- Executes on button press in pushbutton10.
function plotnodes_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton10 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

set(handles.floodnodes, 'Enable', 'off');
set(handles.ExaminePointer, 'Enable', 'off');
set(handles.ExaminePointer, 'Checked', 'off');
set(handles.Path, 'Enable', 'off');
set(handles.Path, 'Checked', 'off');
set(handles.sourcepointer, 'Checked', 'on');

A=0:handles.x_length;
B=(0:handles.y_length)';

axes(handles.axes1);
cla;

axis([0 ceil(A(1,handles.x_length))+1 0 ceil(B(handles.y_length,1))+1]);

plotdata = [];

if (handles.plotmode == 1)
    xs = handles.x_length * rand([1 handles.count]);
    ys = handles.y_length * rand([1 handles.count]);
    for i=1:handles.count
        x=xs(i);
        y=ys(i);
        hold on
        plot(x,y,'s','MarkerSize',5,'MarkerEdgeColor','k','MarkerFaceColor','g','LineWidth',2)
        plotdata(i).parent = -1;
        plotdata(i).X = x;
        plotdata(i).Y = y;
        plotdata(i).child = [];
    end
elseif (handles.plotmode == 0)
    count = 1;
    node_count = handles.count^(0.5);
    node_dir1 = 0;
    node_dir2 = 0;
    if (node_count == floor(node_count))
        node_dir1 = node_count;
        node_dir2 = node_count;
    elseif ((floor(node_count) * ceil(node_count)) >= handles.count)
        node_dir1 = floor(node_count);
        node_dir2 = ceil(node_count);
    else
        node_dir1 = ceil(node_count);
        node_dir2 = node_dir1;
    end
    if (handles.y_length >= handles.x_length)
        node_dist_y = handles.y_length / node_dir2;
        node_dist_x = handles.x_length / node_dir1;
        for height = 0:node_dist_y:(handles.y_length-node_dist_y)
            for length = 0:node_dist_x:(handles.x_length-node_dist_x)
                if (count <= handles.count)
                    x=length + (node_dist_x/2);
                    y=height + (node_dist_y/2);
                    hold on
                    plot(x,y,'s','MarkerSize',5,'MarkerEdgeColor','k',...
                        'MarkerFaceColor','g','LineWidth',2)
                    plotdata(count).parent = -1;
                    plotdata(count).X = x;
                    plotdata(count).Y = y;
                    plotdata(count).child = [];
                    count = count + 1;
                end
            end
        end
    else
        node_dist_x = handles.x_length / node_dir2;
        node_dist_y = handles.y_length / node_dir1;
        for length = 0:node_dist_x:(handles.x_length-node_dist_x)
            for height = 0:node_dist_y:(handles.y_length-node_dist_y)
                if (count <= handles.count)
                    x=length + (node_dist_x/2);
                    y=height + (node_dist_y/2);
                    hold on
                    plot(x,y,'s','MarkerSize',5,'MarkerEdgeColor','k',...
                        'MarkerFaceColor','g','LineWidth',2)
                    plotdata(count).parent = -1;
                    plotdata(count).X = x;
                    plotdata(count).Y = y;
                    plotdata(count).child = [];
                    count = count + 1;
                end
            end
        end
    end
end

sourcenodedata = plotdata(handles.beginnode);
textstring = {strcat('The source node is located at (',...
              num2str(sourcenodedata.X, '%6.2f'),...
              ',', num2str(sourcenodedata.Y, '%6.2f'), ')')};
          
set(handles.statusbar, 'String', textstring);

handles.plotdata = plotdata;

set(handles.floodnodes, 'Enable', 'on');
guidata(handles.figure1, handles);

% --------------------------------------------------------------------
function File_Callback(hObject, eventdata, handles)
% hObject    handle to File (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --------------------------------------------------------------------
function Open_Callback(hObject, eventdata, handles)
% hObject    handle to Open (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

[filename, pathname] = uigetfile('*.mat', 'Open');
if isequal(filename,0)
   disp('User selected Cancel')
else
   filepath = fullfile(pathname, filename);
   data = open(filepath);
   handles.x_length = data.x_length;
   handles.y_length = data.y_length;
   handles.node_dist = data.node_dist;
   handles.count = data.count;
   handles.beginnode = data.beginnode;
   handles.flood_threshold = data.flood_threshold;
   handles.trans_power = data.trans_power;
   handles.factor = data.factor;
   handles.goodreceptiondistance = data.goodreceptiondistance;
   handles.badreceptiondistance = data.badreceptiondistance;
   set(handles.xnodes,'String', handles.x_length);
   set(handles.ynodes,'String', handles.y_length);
   set(handles.nodedistance,'String', handles.node_dist);
   set(handles.nodecount,'String', handles.count);
   set(handles.startnode,'String', handles.beginnode);
   set(handles.threshold,'String', handles.flood_threshold);
   set(handles.power,'String', handles.trans_power);
   set(handles.environmentfactor,'String', handles.factor);
   set(handles.weakrange,'String', handles.badreceptiondistance);
   set(handles.goodrange,'String', handles.goodreceptiondistance);
 
   % Update handles structure
   guidata(handles.figure1, handles);
end


% --- Executes on button press in clearplot.
function clearplot_Callback(hObject, eventdata, handles)
% hObject    handle to clearplot (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

axes(handles.axes1);
cla;

function pausetime_Callback(hObject, eventdata, handles)
% hObject    handle to pausetime (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of pausetime as text
%        str2double(get(hObject,'String')) returns contents of pausetime as a double

waittime = str2double(get(hObject, 'String'));
if isnan(waittime)
    set(hObject, 'String', 2);
    errordlg('Input must be a number','Error');
end
if (waittime < 0) 
    set(hObject, 'String', 2);
    errordlg('Input must be greater than 0','Error');
end
% Save the new x_length value
handles.waittime = str2double(get(hObject, 'String'));
guidata(hObject,handles)

% --- Executes during object creation, after setting all properties.
function pausetime_CreateFcn(hObject, eventdata, handles)
% hObject    handle to pausetime (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

function prob = likelyprobability(distance, handles)
a = handles.goodreceptiondistance;
b = handles.badreceptiondistance;
returnvalue = 0;
if (distance <= a)
    returnvalue = 1;
else
    prob = 100 - ((100/(a-b))*(a-distance));
    randomnum = rand(1) * 100;
    if (randomnum <= prob)
        returnvalue = 1;
    end
end
prob = returnvalue;

function reachdist = reachabledistance(trans_power, threshold, en_factor)
power_transmitted = 10.^(trans_power/10);
power_threshold = 10.^(threshold/10);
reachdist = (power_transmitted / power_threshold)^(1/en_factor);


function weakrange_Callback(hObject, eventdata, handles)
% hObject    handle to weakrange (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of weakrange as text
%        str2double(get(hObject,'String')) returns contents of weakrange as a double

weakrange = str2double(get(hObject, 'String'));
if isnan(weakrange)
    set(hObject, 'String', 250);
    errordlg('Input must be a number','Error');
end
if (weakrange < handles.goodreceptiondistance) 
    set(hObject, 'String', handles.goodreceptiondistance + 100);
    errordlg('Input must be greater than the ensured reception distance','Error');
end
% Save the new x_length value
handles.badreceptiondistance = str2double(get(hObject, 'String'));
guidata(hObject,handles)

% --- Executes during object creation, after setting all properties.
function weakrange_CreateFcn(hObject, eventdata, handles)
% hObject    handle to weakrange (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

function goodrange_Callback(hObject, eventdata, handles)
% hObject    handle to goodrange (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of goodrange as text
%        str2double(get(hObject,'String')) returns contents of goodrange as a double

goodrange = str2double(get(hObject, 'String'));
if isnan(goodrange)
    set(hObject, 'String', 75);
    errordlg('Input must be a number','Error');
end
if (goodrange < 0) 
    set(hObject, 'String', 75);
    errordlg('Input must be greater than 0','Error');
end
% Save the new x_length value
handles.goodreceptiondistance = str2double(get(hObject, 'String'));
if (handles.goodreceptiondistance > handles.badreceptiondistance)
    handles.badreceptiondistance = handles.goodreceptiondistance;
end
guidata(hObject,handles)


% --- Executes during object creation, after setting all properties.
function goodrange_CreateFcn(hObject, eventdata, handles)
% hObject    handle to goodrange (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

% --------------------------------------------------------------------
function exit_program_Callback(hObject, eventdata, handles)
% hObject    handle to exit_program (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

close;

% --------------------------------------------------------------------
function savefile_Callback(hObject, eventdata, handles)
% hObject    handle to savefile (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

[filename, pathname] = uiputfile('*.mat', 'Save');
if isequal(filename,0)
   disp('User selected Cancel')
else
   filepath = fullfile(pathname, filename);
   save(filepath, '-struct', 'handles');
end

% --- Executes on mouse press over axes background.
function axes1_ButtonDownFcn(hObject, eventdata, handles)
% hObject    handle to axes1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

[x, y] = ginput(1);
plotdata = handles.plotdata;
nearplot = 0;
nearx = -100000;
neary = -100000;
neardist = sqrt((x-nearx)^2 + (y-neary)^2);
for j=1:handles.count
    dist = sqrt((x-plotdata(j).X)^2 + (y-plotdata(j).Y)^2);
    if (dist < neardist)
        neardist = dist;
        nearplot = j;
        nearx = plotdata(j).X;
        neary = plotdata(j).Y;
    end
end

examinemode = get(handles.ExaminePointer, 'Checked');
examineon =  get(handles.ExaminePointer, 'Enable');
pathmode = get(handles.Path, 'Checked');
pathon =  get(handles.Path, 'Enable');
sourcemode = get(handles.sourcepointer, 'Checked');

if (strcmp(sourcemode, 'on') == 1)
    set(handles.startnode, 'String', num2str(nearplot));
    handles.beginnode = nearplot;
    sourcenodedata = plotdata(handles.beginnode);
    textstring = {strcat('The source node is located at (',...
                  num2str(sourcenodedata.X, '%6.2f'),...
                 ',', num2str(sourcenodedata.Y, '%6.2f'), ')')};
    set(handles.statusbar, 'String', textstring);
    guidata(hObject,handles)
elseif ((strcmp(examinemode, 'on') == 1) &&...
        (strcmp(examineon, 'on') == 1))
    propwindow = plotproperties('inputnode', nearplot,...
                                'maxdist', handles.badreceptiondistance,...
                                'inputdata', handles.plotdata);
elseif ((strcmp(pathmode, 'on') == 1) &&...
        (strcmp(pathon, 'on') == 1))

%    A=0:handles.x_length;
%    B=(0:handles.y_length)';

%    axes(handles.axes1);
     cla;

%    axis([0 ceil(A(1,handles.x_length))+1 0 ceil(B(handles.y_length,1))+1]);

    for i=1:handles.count
        x=plotdata(i).X;
        y=plotdata(i).Y;
        hold on
        plot(x,y,'s','MarkerSize',5,'MarkerEdgeColor','k','MarkerFaceColor','g','LineWidth',2)
    end
    sourcenodes = handles.beginnode;
    newsourcenodes = [];
    colors = ['c' 'm' 'y' 'r' 'g' 'b' 'k'];
    usecolor = 1;
    
    while (isempty(sourcenodes) < 1)
        arraysize = size(sourcenodes);
    	currnode = sourcenodes(1,1);
        childrennodes = plotdata(currnode).child;
        newsourcenodes = [newsourcenodes childrennodes];
    	numberofchildrennodes = size(childrennodes);
    	for n=1:1:(numberofchildrennodes(1,2));
            node = childrennodes(1,n);
            xs = [plotdata(currnode).X plotdata(node).X];
            ys = [plotdata(currnode).Y plotdata(node).Y];
            plot(xs, ys, colors(1, usecolor), 'LineWidth',2);
        end
        sourcenodes(:,1) = [];
    	if isempty(sourcenodes);
	        usecolor = mod(usecolor+1, 7);
        	if (usecolor == 0)
            	usecolor = 7;
            end
        	sourcenodes = newsourcenodes;
            newsourcenodes = [];
        end
    end
    if (plotdata(nearplot).parent > 0)
        endnode = nearplot;
        while (plotdata(endnode).parent > 0)
	        parentnode = plotdata(endnode).parent;
            xs = [plotdata(endnode).X plotdata(parentnode).X];
            ys = [plotdata(endnode).Y plotdata(parentnode).Y];
            plot(xs, ys, colors(1, 7), 'LineWidth',3);
            endnode = parentnode;
        end
    end
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


% --- Executes on selection change in distributionmenu.
function distributionmenu_Callback(hObject, eventdata, handles)
% hObject    handle to distributionmenu (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = get(hObject,'String') returns distributionmenu contents as cell array
%        contents{get(hObject,'Value')} returns selected item from distributionmenu

val = get(hObject,'Value');
switch val
    case 2
        handles.plotmode = 0;
    case 3
        handles.plotmode = 1;
    otherwise
        handles.plotmode = 0;
        set(hObject,'Value',2);
end
guidata(hObject,handles)

% --- Executes during object creation, after setting all properties.
function distributionmenu_CreateFcn(hObject, eventdata, handles)
% hObject    handle to distributionmenu (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end




% --------------------------------------------------------------------
function pointer_menu_Callback(hObject, eventdata, handles)
% hObject    handle to pointer_menu (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --------------------------------------------------------------------
function sourcepointer_Callback(hObject, eventdata, handles)
% hObject    handle to sourcepointer (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

set(hObject, 'Checked', 'on');
examinestatus = get(handles.ExaminePointer, 'Enable');
pathstatus  = get(handles.Path, 'Enable');
if (strcmp(examinestatus, 'on') == 1)
    set(handles.ExaminePointer, 'Checked', 'off');
end
if (strcmp(pathstatus, 'on') == 1)
    set(handles.Path, 'Checked', 'off');
end
guidata(hObject,handles)
% --------------------------------------------------------------------
function ExaminePointer_Callback(hObject, eventdata, handles)
% hObject    handle to ExaminePointer (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(hObject, 'Checked', 'on');
set(handles.sourcepointer, 'Checked', 'off');
set(handles.Path, 'Checked', 'off');
guidata(hObject,handles)


% --------------------------------------------------------------------
function Path_Callback(hObject, eventdata, handles)
% hObject    handle to Path (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(hObject, 'Checked', 'on');
set(handles.sourcepointer, 'Checked', 'off');
set(handles.ExaminePointer, 'Checked', 'off');
guidata(hObject,handles)







% --- Executes on selection change in listbox1.
function listbox1_Callback(hObject, eventdata, handles)
% hObject    handle to listbox1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = get(hObject,'String') returns listbox1 contents as cell array
%        contents{get(hObject,'Value')} returns selected item from listbox1


% --- Executes during object creation, after setting all properties.
function listbox1_CreateFcn(hObject, eventdata, handles)
% hObject    handle to listbox1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on selection change in popupmenu2.
function popupmenu2_Callback(hObject, eventdata, handles)
% hObject    handle to popupmenu2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = get(hObject,'String') returns popupmenu2 contents as cell array
%        contents{get(hObject,'Value')} returns selected item from popupmenu2


% --- Executes during object creation, after setting all properties.
function popupmenu2_CreateFcn(hObject, eventdata, handles)
% hObject    handle to popupmenu2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


