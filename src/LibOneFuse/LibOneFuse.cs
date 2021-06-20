using System;
using System.Runtime.InteropServices;

using System.Threading.Tasks;

using Microsoft.Identity.Client;

using Microsoft.Graph;
using Microsoft.Graph.Auth;

namespace LibOneFuse
{
    public sealed class GraphClientSingleton
    {
        private static readonly GraphClientSingleton instance = new GraphClientSingleton();

        private GraphServiceClient client;

        // Explicit static constructor to tell C# compiler
        // not to mark type as beforefieldinit
        static GraphClientSingleton()
        {
        }

        private GraphClientSingleton()
        {
            IPublicClientApplication publicClientApplication = PublicClientApplicationBuilder
                .Create("INSERT-CLIENT-APP-ID")
                .Build();
            
            Func<DeviceCodeResult, Task> deviceCodeReadyCallback = async dcr => await Console.Out.WriteLineAsync(dcr.Message);

            string[] graphScopes = {"Files.ReadWrite.All"};
            DeviceCodeProvider authProvider = new DeviceCodeProvider(publicClientApplication,graphScopes,deviceCodeReadyCallback);

            client = new GraphServiceClient(authProvider);
        }

        public static GraphClientSingleton Instance
        {
            get
            {
                return instance;
            }
        }

        public GraphServiceClient Client
        { 
            get
            {
                return client;
            }
        }
    
    }
    
    public class LibOneFuse
    {



        [UnmanagedCallersOnly(EntryPoint="onefuse_listdir")]
        public static bool ListDir(IntPtr path_ptr)
        {
            string path = Marshal.PtrToStringUTF8(path_ptr);
            
            var driveItem = GraphClientSingleton.Instance.Client.Me.Drive.Root.ItemWithPath(path).Request().GetAsync();
            driveItem.Wait(-1);
            if( driveItem.Result.Folder == null )
            {
                // returned item is not a folder
                return false;
            }

            var children = driveItem.Result.Children;
            foreach( var child in children )
            {
                // Append new directory entry to result
            }

            return true;
        }

        // Initialise library
        [UnmanagedCallersOnly(EntryPoint="init")]
        public static bool Init()
        {
        
            return true;
        }

        // Teardown library
        [UnmanagedCallersOnly(EntryPoint="finalise")]
        public static bool Finalise()
        {
            return true;
        }

    }
}