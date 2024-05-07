function varargout = mwSlider(varargin)
% MWSLIDER M-file for mwSlider.fig
%      MWSLIDER, by itself, creates a new MWSLIDER or raises the existing
%      singleton*.
%
%      H = MWSLIDER returns the handle to a new MWSLIDER or the handle to
%      the existing singleton*.
%
%      MWSLIDER('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in MWSLIDER.M with the given input arguments.
%
%      MWSLIDER('Property','Value',...) creates a new MWSLIDER or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before mwSlider_OpeningFunction gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to mwSlider_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help mwSlider

% Last Modified by GUIDE v2.5 12-Apr-2004 15:08:23

% Change path accordingly in two occurences

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @mwSlider_OpeningFcn, ...
                   'gui_OutputFcn',  @mwSlider_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin & isstr(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


% --- Executes just before mwSlider is made visible.
function mwSlider_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to mwSlider (see VARARGIN)

% Choose default command line output for mwSlider
handles.output = hObject;

handles.cc = [];    % CCSDSP handle
handles = initialize_parameters(handles);
load('c:\CCStudio_v3.1\myprojects\rtdx_matlabFIR\filtercoeffs.mat');
handles.coeffs = coeffs;
set(handles.stop,'Userdata',false);
set(handles.filterCoeffs,'Userdata',0);
set(handles.signal,...
    'xlim',[1 512],...
    'ylim',[-1000 1000]...
    );
set(handles.spectrum,...
    'xlim',[1 128],...
    'ylim',[-50 0]...
    );

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes mwSlider wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = mwSlider_OutputFcn(hObject, eventdata, handles)
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes on button press in selectTarget.
function selectTarget_Callback(hObject, eventdata, handles)
% hObject    handle to selectTarget (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

[bn,pn] = boardprocsel; drawnow;
handles.cc = ccsdsp('boardnum',bn,'procnum',pn);

% Update handles structure
guidata(hObject, handles);

% --- Executes on button press in stop.
function stop_Callback(hObject, eventdata, handles)
set(hObject,'Userdata',true);
get(hObject,'Userdata');
% Update handles structure
guidata(hObject, handles);


% --- Executes during object creation, after setting all properties.
function filterCoeffs_CreateFcn(hObject, eventdata, handles)
usewhitebg = 1;
if usewhitebg
    set(hObject,'BackgroundColor',[.9 .9 .9]);
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end


% --- Executes on slider movement.
function filterCoeffs_Callback(hObject, eventdata, handles)
% handles.cc.rtdx.writemsg('ichan',int16(get(hObject,'Value')));
cutoff = [600 1500 3000];
val = get(hObject,'Value');
tmp = floor(get(hObject,'Value'));
if mod(val,tmp)>0.5
    newval = floor(val)+1;
else
    newval = floor(val);
end
set(hObject,'Value',newval);
if isempty(handles.cc) 
    errordlg('You have not selected a target.','No target selected','modal');
elseif ~handles.cc.rtdx.isenabled
    errordlg('You must first click ''Start'' to do this.','Demo not started','modal');
else
    handles.cc.rtdx.writemsg('ichan',int16(handles.coeffs(newval+1,:)));
    set(handles.filterCoeffs,'Userdata',newval);
end

% --- Executes on button press in run.
function run_Callback(hObject, eventdata, handles)

if isempty(handles.cc)
    errordlg('You must select a target before running the demo.','No target selected','modal');
    return;
end

cc = handles.cc;
r = cc.rtdx;
visible(cc,1); % for debugging purposes

factr = 2^15;
N = 256; % data samples per message
t = 1:N;
N2 = N/2;
fVect = ( 0:(1/N2):1-(1/N2) )';

try
    cc.reset;
    pause(1);
    
    r.disable; % disable RTDX
    
    cc.cd('c:\CCStudio_v3.1\myprojects\rtdx_matlabFIR');
    warning off;  % disable warning when opening project 
    cc.open('RTDX_MATLABFIR.pjt'); 
    warning on;  
    cc.load('Debug\RTDX_MATLABFIR.out');
    cc.run;
    pause(0.5);

    % configure RTDX
    r.disable;
    r.open('ichan','w');
    r.open('ochan','r');
    r.enable('ichan');
    r.enable('ochan');
    r.configure(64000,4,'continuous');
    r.enable;

    % initial value
    halt = false;
    initval = get(handles.filterCoeffs,'Value') + 1;
    r.writemsg('ichan',int16(handles.coeffs(initval,:)));
    axis tight;
    
    while ~halt,
        
        % read data from DSK
        data_frame = double(r.readmsg('ochan', 'int32'));
        data_frame = data_frame/factr;
        
        % plot data
        plot(t,data_frame,'Parent',handles.signal); grid(handles.signal);
        axis tight;
        
		% Compute spectrum and plot
		spectrum_tgt = fft(data_frame);
		halfSpect_tgt = db(spectrum_tgt(1:length(spectrum_tgt)/2));
		halfSpect_tgt = halfSpect_tgt - max(halfSpect_tgt);
        plot(fVect,halfSpect_tgt,'Parent',handles.spectrum); grid(handles.spectrum);
        axis tight;
        
        halt = get(handles.stop,'Userdata');
        if ~isrunning(cc)
            halt = set(handles.stop,'Userdata',true);
        end

        pause(0.01);   % needed to get user-GUI input
        
    end

    CleanupRTDX(cc,r);
    
catch
    
    CleanupRTDX(cc,r);
    error(lasterr);

end

handles = initialize_parameters(handles);

%------------------------------
function CleanupRTDX(cc,r)
cc.halt;                % halt processor
flush(r,'ochan','all')  % flush all remaining messages
flush(r,'ichan')        % flush all remaining messages
r.disable('ichan');     % disable channel 'ichan'
r.disable('ochan');     % disable channel 'ochan'
r.close('ichan');       % close channel 'ichan'
r.close('ochan');       % close channel 'ochan'
r.disable;              % disable RTDX

%--------------------------------------------------------------------------
function handles = initialize_parameters(handles)
set(handles.filterCoeffs,'Min',0);
set(handles.filterCoeffs,'Max',2);
% Initialize
set(handles.filterCoeffs,'Userdata',0);
set(handles.stop,'Userdata',false);
% Update handles structure
guidata(handles.filterCoeffs, handles);